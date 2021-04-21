/*
 * chardev.c: Creates a read-only char device that says how many times you've
 * read from the dev file.
 *
 * You can have some fun with this by removing the module_get/put calls,
 * allowing the module to be removed while the file is still open.
 *
 * Compile with `make`. Load with `sudo insmod chardev.ko`. Check `dmesg | tail`
 * output to see the assigned device number and command to create a device file.
 *
 * From TLDP.org's LKMPG book.
 * 
 * From: https://gist.github.com/brenns10/65d1ee6bb8419f96d2ae693eb7a66cc0
 * From: https://www.linuxjournal.com/article/2920
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <asm/uaccess.h> /* for put_user */

#define MODULE_NAME_LOG "my_mdev: "

/*
 * Prototypes - this would normally go in a .h file
 */

int init_module(void);
void cleanup_module(void);
static long device_ioctl(struct file *file, unsigned int request, unsigned long data);

#define SUCCESS 0
#define DEVICE_NAME "mchardev"
#define BUF_LEN 80

/*
 * Global variables are declared as static, so are global within the file.
 */
static struct file_operations fops = {
    .unlocked_ioctl = device_ioctl};

static struct miscdevice mdev = {
    .minor = 0,
    .name = DEVICE_NAME,
    .mode = S_IALLUGO,
    .fops = &fops};

/*
 * This function is called when the module is loaded
 */
int init_module(void)
{
    int ret;
    printk(KERN_DEBUG MODULE_NAME_LOG "init\n");

    ret = misc_register(&mdev);

    if (ret < 0)
    {
        printk(KERN_ALERT MODULE_NAME_LOG "Registering char device failed\n");
        return ret;
    }

    printk(KERN_DEBUG MODULE_NAME_LOG "Device registered successfully\n");

    return SUCCESS;
}

/*
 * This function is called when the module is unloaded
 */
void cleanup_module(void)
{
    /*
     * Unregister the device
     */
    misc_deregister(&mdev);

    printk(KERN_DEBUG MODULE_NAME_LOG "exit\n");
}

static long device_ioctl(struct file *file, unsigned int request, unsigned long data)
{
    printk(KERN_DEBUG MODULE_NAME_LOG "device_ioctl: pid->%d, path=%s, request=%u\n", current->pid, file->f_path.dentry->d_iname, request);

    return SUCCESS;
}