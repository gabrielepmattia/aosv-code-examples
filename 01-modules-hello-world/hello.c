#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriele Proietti Mattia <pm.gabriele@outlook.com>");
MODULE_DESCRIPTION("My first hello world module");
MODULE_VERSION("1.0.0");

static int __init dummy_init(void)
{
    printk(KERN_INFO "Hello, this is my first kernel module \n");
    return 0;
}

static void __exit dummy_exit(void)
{
    printk(KERN_INFO "Exit Hello world module\n");
}

module_init(dummy_init);
module_exit(dummy_exit);