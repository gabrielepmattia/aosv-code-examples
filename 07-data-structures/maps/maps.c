#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/idr.h>

MODULE_DESCRIPTION("Linked List example");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriele Proietti Mattia <pm.gabriele@outlook.com>");

#define MODULE_NAME_LOG "my_queues: "

#define MAX_ID 10
#define MIN_ID 0

typedef struct my_list_item
{
        int id;
        int my_field1;
        int my_field2;
} my_list_item;

int process_item(int id, void *p, void *data)
{       
        my_list_item* item = p;
        printk(KERN_DEBUG MODULE_NAME_LOG "processing assigned_id=%d, item->id=%d, &item=%p\n", id, item->id, p);
        return 0;
}

static int my_module_init(void)
{
        struct idr my_idr;
        int id1, id2, id3;
        my_list_item *item1 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item *item2 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item *item3 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item *item_ptr;

        printk(KERN_DEBUG MODULE_NAME_LOG "init\n");

        // init
        idr_init(&my_idr);

        item1->id = 0;
        item1->my_field1 = 0;
        item1->my_field2 = 0;

        item2->id = 1;
        item2->my_field1 = 0;
        item2->my_field2 = 0;

        item3->id = 2;
        item3->my_field1 = 0;
        item3->my_field2 = 0;

        // add
        id1 = idr_alloc(&my_idr, item1, MIN_ID, MAX_ID, GFP_KERNEL);
        id2 = idr_alloc(&my_idr, item2, MIN_ID, MAX_ID, GFP_KERNEL);
        id3 = idr_alloc(&my_idr, item3, MIN_ID, MAX_ID, GFP_KERNEL);

        printk(KERN_DEBUG MODULE_NAME_LOG "&item1=%p, assigned_id1=%d\n", item1, id1);
        printk(KERN_DEBUG MODULE_NAME_LOG "&item2=%p, assigned_id2=%d\n", item2, id2);
        printk(KERN_DEBUG MODULE_NAME_LOG "&item3=%p, assigned_id3=%d\n", item3, id3);

        // retrieve
        idr_for_each(&my_idr, &process_item, NULL);

        // remove
        item_ptr = idr_remove(&my_idr, id1);
        printk(KERN_DEBUG MODULE_NAME_LOG "removed item->id=%d &item=%p\n", item_ptr->id, item_ptr);

        item_ptr = idr_remove(&my_idr, id2);
        printk(KERN_DEBUG MODULE_NAME_LOG "removed item->id=%d &item=%p\n", item_ptr->id, item_ptr);

        item_ptr = idr_remove(&my_idr, id3);
        printk(KERN_DEBUG MODULE_NAME_LOG "removed item->id=%d &item=%p\n", item_ptr->id, item_ptr);

        kfree(item1);
        kfree(item2);
        kfree(item3);

        // remember to free the queue after usage
        idr_destroy(&my_idr);

        return 0;
}

static void my_module_exit(void)
{
        printk(KERN_DEBUG MODULE_NAME_LOG "exit\n");
}

module_init(my_module_init);
module_exit(my_module_exit);