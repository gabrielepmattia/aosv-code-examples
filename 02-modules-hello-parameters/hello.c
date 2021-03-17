#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#define MODULE_LOG "hello: "

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriele Proietti Mattia <pm.gabriele@outlook.com>");
MODULE_DESCRIPTION("My first hello world module");
MODULE_VERSION("1.0.0");

int myint = -1;
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");

static int __init dummy_init(void)
{
    printk(KERN_INFO "Hello, this is my first kernel module \n");

    printk(KERN_INFO MODULE_LOG "this is a correct print\n");
    printk(KERN_WARNING MODULE_LOG "Variable myint = %d\n", myint);

    return 0;
}

static void __exit dummy_exit(void)
{
    printk(KERN_INFO MODULE_LOG "module exited\n");
}

module_init(dummy_init);
module_exit(dummy_exit);