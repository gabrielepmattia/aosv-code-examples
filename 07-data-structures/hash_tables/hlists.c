#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/hashtable.h>
#include <linux/slab.h>

MODULE_DESCRIPTION("Linked List example");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriele Proietti Mattia <pm.gabriele@outlook.com>");

#define MODULE_NAME_LOG "my_hlists: "

#define MY_HASH_BITS 4

typedef struct my_list_item
{
        int id;
        int my_field1;
        struct hlist_node list;
        int my_field2;
} my_list_item;

static int my_module_init(void)
{
        DEFINE_HASHTABLE(my_hash, MY_HASH_BITS);
        my_list_item *item1 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item *item2 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item *item3 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item *current_item;
        int current_bucket;

        printk(KERN_DEBUG MODULE_NAME_LOG "init\n");

        // init hash list
        hash_init(my_hash);

        item1->id = 0;
        item1->my_field1 = 0;
        item1->my_field2 = 0;

        item2->id = 1;
        item2->my_field1 = 0;
        item2->my_field2 = 0;

        item3->id = 2;
        item3->my_field1 = 0;
        item3->my_field2 = 0;

        // add items to list
        hash_add(my_hash, &item1->list, item1->id);
        hash_add(my_hash, &item2->list, item2->id);
        hash_add(my_hash, &item3->list, item3->id);

        printk(KERN_DEBUG MODULE_NAME_LOG "&item1=%p\n", item1);
        printk(KERN_DEBUG MODULE_NAME_LOG "&item2=%p\n", item2);
        printk(KERN_DEBUG MODULE_NAME_LOG "&item3=%p\n", item3);

        // traverse list
        hash_for_each(my_hash, current_bucket, current_item, list)
        {
                printk(KERN_DEBUG MODULE_NAME_LOG "&current_item=%p, current_item->id=%d\n", current_item, current_item->id);
        }

        // delete the items from the table
        hash_del(&item1->list);
        hash_del(&item2->list);
        hash_del(&item3->list);

        kfree(item1);
        kfree(item2);
        kfree(item3);

        return 0;
}

static void my_module_exit(void)
{
        printk(KERN_DEBUG MODULE_NAME_LOG "exit\n");
}

module_init(my_module_init);
module_exit(my_module_exit);