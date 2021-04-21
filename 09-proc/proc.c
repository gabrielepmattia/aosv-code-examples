#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#define BUFSIZE 100

/*
 * See full example at https://devarea.com/linux-kernel-development-creating-a-proc-file-and-interfacing-with-user-space/
 */

MODULE_DESCRIPTION("proc example");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriele Proietti Mattia <pm.gabriele@outlook.com>");

#define MODULE_NAME_LOG "my_proc: "

static ssize_t myproc_read(struct file *file, char __user *ubuf, size_t count, loff_t *offset);
static ssize_t myproc_write(struct file *file, const char __user *ubuf, size_t count, loff_t *offset);

static struct proc_dir_entry *ent;

static struct proc_ops pops =
    {
        .proc_read = myproc_read,
        .proc_write = myproc_write,
};

static int my_module_init(void)
{
        printk(KERN_DEBUG MODULE_NAME_LOG "init");

        ent = proc_create("myprocfile", S_IALLUGO, NULL, &pops);

        return 0;
}

static void my_module_exit(void)
{
        printk(KERN_DEBUG MODULE_NAME_LOG "exit");

        proc_remove(ent);
}

/*
 * Methods
 */

static ssize_t myproc_write(struct file *file, const char __user *ubuf, size_t count, loff_t *ppos)
{
        printk(KERN_DEBUG MODULE_NAME_LOG "write: pid->%d, length=%ld, offset=%llu,\n", current->pid, count, *ppos);

        printk(KERN_ALERT MODULE_NAME_LOG "Sorry, this operation isn't supported.\n");
        return -EINVAL;
}

static ssize_t myproc_read(struct file *file, char __user *ubuf, size_t count, loff_t *ppos)
{
        char buf[BUFSIZE];
        int len = 0;

        printk(KERN_DEBUG MODULE_NAME_LOG "read: pid->%d, length=%ld, offset=%llu,\n", current->pid, count, *ppos);

        if (*ppos > 0 || count < BUFSIZE)
                return 0;
        len += sprintf(buf, "this is a test proc file, pid=%d\n", current->pid);

        if (copy_to_user(ubuf, buf, len))
                return -EFAULT;
        *ppos = len;

        return len;
}

module_init(my_module_init);
module_exit(my_module_exit);