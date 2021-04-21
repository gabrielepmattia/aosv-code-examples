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
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);

#define SUCCESS 0
#define DEVICE_NAME "mchardev"
#define BUF_LEN 80

/*
 * Global variables are declared as static, so are global within the file.
 */

static int Major;
static int Device_Open = 0;
static char msg[BUF_LEN];
static char *msg_Ptr;

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release};

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
        printk(KERN_ALERT MODULE_NAME_LOG "Registering char device failed with %d\n", Major);
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

/*
 * Methods
 */

/*
 * Called when a process tries to open the device file, like
 * "cat /dev/mycharfile"
 */
static int device_open(struct inode *inode, struct file *filp)
{
    static int counter = 0;

    printk(KERN_DEBUG MODULE_NAME_LOG "open: pid->%d, counter->%d\n", current->pid, counter);

    if (Device_Open)
        return -EBUSY;

    Device_Open++;
    sprintf(msg, "I already told you %d times Hello world!\n", counter++);
    msg_Ptr = msg;
    /*
   * TODO: comment out the line below to have some fun!
   */
    try_module_get(THIS_MODULE);

    return SUCCESS;
}

/*
 * Called when a process closes the device file.
 */
static int device_release(struct inode *inode, struct file *filp)
{
    printk(KERN_DEBUG MODULE_NAME_LOG "release: pid->%d\n", current->pid);

    Device_Open--;

    /*
   * Decrement the usage count, or else once you opened the file, you'll never
   * get rid of the module.
   *
   * TODO: comment out the line below to have some fun!
   */
    module_put(THIS_MODULE);

    return SUCCESS;
}

/*
 * Called when a process, which already opened the dev file, attempts to read
 * from it.
 */
static ssize_t device_read(struct file *filp, /* see include/linux/fs.h   */
                           char *buffer,      /* buffer to fill with data */
                           size_t length,     /* length of the buffer     */
                           loff_t *offset)
{
    /*
   * Number of bytes actually written to the buffer
   */
    int bytes_read = 0;

    printk(KERN_DEBUG MODULE_NAME_LOG "read: pid->%d, length=%ld, offset=%llu, *msg_Ptr=%d\n", current->pid, length, *offset, *msg_Ptr);

    /*
   * If we're at the end of the message, return 0 signifying end of file.
   */
    if (*msg_Ptr == 0)
        return 0;

    /*
   * Actually put the data into the buffer
   */
    while (length && *msg_Ptr)
    {
        /*
     * The buffer is in the user data segment, not the kernel segment so "*"
     * assignment won't work. We have to use put_user which copies data from the
     * kernel data segment to the user data segment.
     */
        put_user(*(msg_Ptr++), buffer++);
        length--;
        bytes_read++;
    }

    /*
   * Most read functions return the number of bytes put into the buffer
   */
    return bytes_read;
}

/*
 * Called when a process writes to dev file: echo "hi" > /dev/hello
 */
static ssize_t
device_write(struct file *filp, const char *buf, size_t len, loff_t *off)
{
    printk(KERN_DEBUG MODULE_NAME_LOG "write: pid->%d\n", current->pid);
    
    printk(KERN_ALERT "Sorry, this operation isn't supported.\n");
    return -EINVAL;
}
