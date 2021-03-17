#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriele Proietti Mattia <pm.gabriele@outlook.com>");
MODULE_DESCRIPTION("My first hello world module");
MODULE_VERSION("1.0.0");

short myshort;
module_param(myshort, short, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
MODULE_PARM_DESC(myshort, "A short integer");

short myint;
module_param(myint, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(myint, "An integer");

short mylong;
module_param(mylong, long, S_IRUSR);
MODULE_PARM_DESC(mylong, "A long integer");

short mystring;
module_param(mystring, charp, 0000);
MODULE_PARM_DESC(mystring, "A character string");

short myintarray;
module_param_array(myintArray, int, &arr_argc, 0000);
MODULE_PARM_DESC(myintArray, "An array of integers");

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