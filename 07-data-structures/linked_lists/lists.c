#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>

MODULE_DESCRIPTION("Linked List example");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Gabriele Proietti Mattia <pm.gabriele@outlook.com>");

#define MODULE_NAME_LOG "my_module: "

typedef struct my_list_item 
{
        int id;
        int my_field1;
        struct list_head list;
        int my_field2;
} my_list_item;

static int
my_module_init(void)
{
        struct list_head my_list;
        my_list_item* item1 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item* item2 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        my_list_item* item3 = kmalloc(sizeof(my_list_item), GFP_KERNEL);
        struct list_head* current_item_list;
        my_list_item* current_item;
        
        // init list head
        INIT_LIST_HEAD(&my_list);

        item1->id = 0;
        item1->my_field1 = 0;
        item1->my_field2 = 0;

        item2->id = 1;
        item2->my_field1 = 0;
        item2->my_field2 = 0;

        item3->id = 3;
        item3->my_field1 = 0;
        item3->my_field2 = 0;

        // add items to list
        list_add(&my_list, &item1->list);
        list_add(&my_list, &item2->list);
        list_add(&my_list, &item3->list);

        printk(KERN_DEBUG MODULE_NAME_LOG "&item1=%p", item1);
        printk(KERN_DEBUG MODULE_NAME_LOG "&item2=%p", item2);
        printk(KERN_DEBUG MODULE_NAME_LOG "&item3=%p", item3);

        // traverse list
        list_for_each(current_item_list, &my_list) {
                current_item = list_entry(current_item_list, my_list_item, list);
                printk(KERN_DEBUG MODULE_NAME_LOG "&current_item=%p, current_item->id=%d", current_item, current_item->id);
        }

        kfree(item1);
        kfree(item2);
        kfree(item3);

        return 0;
}

static void my_module_exit(void)
{
}

module_init(my_module_init);
module_exit(my_module_exit);