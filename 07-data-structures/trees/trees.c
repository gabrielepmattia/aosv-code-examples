#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kfifo.h>

MODULE_DESCRIPTION("Linked List example");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriele Proietti Mattia <pm.gabriele@outlook.com>");

#define MODULE_NAME_LOG "my_queues: "

typedef struct my_list_item
{
        int id;
        int my_field1;
        int my_field2;
} my_list_item;

static int my_module_init(void)
{
        struct kfifo fifo;
        int ret;
        my_list_item *item1 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item *item2 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item *item3 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item *current_item;

        printk(KERN_DEBUG MODULE_NAME_LOG "init\n");

        // init queue
        ret = kfifo_alloc(&fifo, PAGE_SIZE, GFP_KERNEL);
        if(ret < 0) {
                printk(KERN_ERR MODULE_NAME_LOG "cannot allocate the fifo queue\n");
                return 1;
        }

        item1->id = 0;
        item1->my_field1 = 0;
        item1->my_field2 = 0;

        item2->id = 1;
        item2->my_field1 = 0;
        item2->my_field2 = 0;

        item3->id = 2;
        item3->my_field1 = 0;
        item3->my_field2 = 0;

        // enqueue (the command copies memory)
        ret = kfifo_in(&fifo, item1, sizeof(my_list_item));
        if(ret < 0) {
                printk(KERN_ERR MODULE_NAME_LOG "cannot add item1 to the fifo queue\n");
                return 1;
        }
        ret = kfifo_in(&fifo, item2, sizeof(my_list_item));
        if(ret < 0) {
                printk(KERN_ERR MODULE_NAME_LOG "cannot add item1 to the fifo queue\n");
                return 1;
        }
        ret = kfifo_in(&fifo, item3, sizeof(my_list_item));
        if(ret < 0) {
                printk(KERN_ERR MODULE_NAME_LOG "cannot add item1 to the fifo queue\n");
                return 1;
        }

        printk(KERN_DEBUG MODULE_NAME_LOG "&item1=%p\n", item1);
        printk(KERN_DEBUG MODULE_NAME_LOG "&item2=%p\n", item2);
        printk(KERN_DEBUG MODULE_NAME_LOG "&item3=%p\n", item3);

        // statistics
        printk(KERN_DEBUG MODULE_NAME_LOG "kfifo_size=%u\n", kfifo_size(&fifo));
        printk(KERN_DEBUG MODULE_NAME_LOG "kfifo_len=%u\n", kfifo_len(&fifo));
        printk(KERN_DEBUG MODULE_NAME_LOG "kfifo_avail=%u\n", kfifo_avail(&fifo));

        // dequeue
        kfifo_out(&fifo, current_item, sizeof(my_list_item));
        printk(KERN_DEBUG MODULE_NAME_LOG "popped item->id=%d &item=%p\n", current_item->id, &current_item);

        kfifo_out(&fifo, current_item, sizeof(my_list_item));
        printk(KERN_DEBUG MODULE_NAME_LOG "popped item->id=%d &item=%p\n", current_item->id, &current_item);
        
        kfifo_out(&fifo, current_item, sizeof(my_list_item));
        printk(KERN_DEBUG MODULE_NAME_LOG "popped item->id=%d &item=%p\n", current_item->id, &current_item);

        kfree(item1);
        kfree(item2);
        kfree(item3);

        // remember to free the queue after usage
        kfifo_free(&fifo);

        return 0;
}

static void my_module_exit(void)
{
        printk(KERN_DEBUG MODULE_NAME_LOG "exit\n");
}

module_init(my_module_init);
module_exit(my_module_exit);