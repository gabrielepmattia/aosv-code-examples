#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/ftrace.h>
#include <linux/kprobes.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

/*
 * See full example at https://www.apriorit.com/dev-blog/546-hooking-linux-functions-2
 */

MODULE_DESCRIPTION("ftrace hooking example");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriele Proietti Mattia <pm.gabriele@outlook.com>");

#define MODULE_NAME_LOG "my_ftrace: "

#define HOOK(_name, _function, _original) \
        {                                 \
                .name = (_name),          \
                .function = (_function),  \
                .original = (_original),  \
        }

/**
 * struct ftrace_hook    describes the hooked function
 *
 * @name: the name of the hooked function
 * @function: the address of the wrapper function that will be called instead of the hooked function
 * @original: a pointer to the place where the address 
 *            of the hooked function should be stored, filled out during installation of the hook
 * @address: the address of the hooked function, filled out during installation of the hook
 * @ops: ftrace service information, initialized by zeros; initialization is finished during installation of the hook
 */
struct ftrace_hook
{
        const char *name;
        void *function;
        void *original;

        unsigned long address;
        struct ftrace_ops ops;
};

/*
 * It’s a pointer to the original system call handler execve().
 * It can be called from the wrapper. It’s extremely important to keep the function signature
 * without any changes: the order, types of arguments, returned value,
 * and ABI specifier (pay attention to “asmlinkage”).
 */
static asmlinkage long (*real_sys_execve)(const char __user *filename,
                                          const char __user *const __user *argv,
                                          const char __user *const __user *envp);

struct filename *(*real_getname)(const char __user *);

/*
 * This function will be called instead of the hooked one. Its arguments are
 * the arguments of the original function. Its return value will be passed on to
 * the calling function. This function can execute arbitrary code before, after, 
 * or instead of the original function.
 */
static asmlinkage long fh_sys_execve(const char __user *filename,
                                     const char __user *const __user *argv,
                                     const char __user *const __user *envp)
{
        long ret;
        // struct filename *fname;
        //char filename_buf[256];
        // int ret2;

        // retrieve the executable name
        // fname = real_getname(filename);
        // printk(KERN_DEBUG MODULE_NAME_LOG "fh_sys_execve: fname=%p\n", fname);

        // ret2 = strncpy_from_user(filename_buf, filename, 256);
        // printk(KERN_DEBUG MODULE_NAME_LOG "fh_sys_execve: copied=%d str=%s\n", ret2, filename_buf);

        printk(KERN_DEBUG MODULE_NAME_LOG "execve() called: filename=%p argv=%p envp=%p\n",
               filename, argv, envp);

        ret = real_sys_execve(filename, argv, envp);

        printk(KERN_DEBUG MODULE_NAME_LOG "execve() returned: %ld\n", ret);

        // kfree(fname);

        return ret;
}

static unsigned long resolve_symbol(const char *symbol_name)
{
        struct kprobe kp;

        // exploit kprobe to find the address
        memset(&kp, 0, sizeof(kp));
        kp.symbol_name = symbol_name;
        if (!register_kprobe(&kp))
        {
                unregister_kprobe(&kp);
        }

        printk(KERN_DEBUG MODULE_NAME_LOG "resolve_symbol: %s resolved to %p\n", symbol_name, (void *)kp.addr);

        return (unsigned long)kp.addr;
}

static int resolve_hook_address(struct ftrace_hook *hook)
{
        // no more working on kernel 5+
        // hook->address = kallsyms_lookup_name(hook->name);

        hook->address = resolve_symbol(hook->name);

        if (!hook->address)
        {
                printk(KERN_ALERT MODULE_NAME_LOG "unresolved symbol: %s\n", hook->name);
                return -ENOENT;
        }

        *((unsigned long *)hook->original) = hook->address;

        return 0;
}

static void notrace fh_ftrace_thunk(unsigned long ip, unsigned long parent_ip,
                                    struct ftrace_ops *ops, struct pt_regs *regs)
{
        struct ftrace_hook *hook = container_of(ops, struct ftrace_hook, ops);

        /* Skip the function calls from the current module. */
        if (!within_module(parent_ip, THIS_MODULE))
                regs->ip = (unsigned long)hook->function;
}

int fh_install_hook(struct ftrace_hook *hook)
{
        int err;

        err = resolve_hook_address(hook);
        if (err)
                return err;

        hook->ops.func = fh_ftrace_thunk;
        hook->ops.flags = FTRACE_OPS_FL_SAVE_REGS | FTRACE_OPS_FL_IPMODIFY;

        err = ftrace_set_filter_ip(&hook->ops, hook->address, 0, 0);
        if (err)
        {
                printk(KERN_ALERT MODULE_NAME_LOG "ftrace_set_filter_ip() failed: %d\n", err);
                return err;
        }

        err = register_ftrace_function(&hook->ops);
        if (err)
        {
                printk(KERN_ALERT MODULE_NAME_LOG "register_ftrace_function() failed: %d\n", err);

                /* Don’t forget to turn off ftrace in case of an error. */
                ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);

                return err;
        }

        return 0;
}

void fh_remove_hook(struct ftrace_hook *hook)
{
        int err;

        err = unregister_ftrace_function(&hook->ops);
        if (err)
        {
                printk(KERN_ALERT MODULE_NAME_LOG "unregister_ftrace_function() failed: %d\n", err);
        }

        err = ftrace_set_filter_ip(&hook->ops, hook->address, 1, 0);
        if (err)
        {
                printk(KERN_ALERT MODULE_NAME_LOG "ftrace_set_filter_ip() failed: %d\n", err);
        }
}

/*
 * Module init/exit
 */

static struct ftrace_hook hooked_functions[] = {
    //      HOOK("sys_clone",   fh_sys_clone,   &real_sys_clone),
    HOOK("__x64_sys_execve", fh_sys_execve, &real_sys_execve),
};

static int my_module_init(void)
{
        printk(KERN_DEBUG MODULE_NAME_LOG "init\n");

        // resolve getname
        // real_getname = (void *)resolve_symbol("getname");
        // printk(KERN_DEBUG MODULE_NAME_LOG "init: getname address is %p, %p\n", real_getname, (void *)resolve_symbol("getname"));

        fh_install_hook(&hooked_functions[0]);

        return 0;
}

static void my_module_exit(void)
{
        printk(KERN_DEBUG MODULE_NAME_LOG "exit\n");

        fh_remove_hook(&hooked_functions[0]);
}

module_init(my_module_init);
module_exit(my_module_exit);