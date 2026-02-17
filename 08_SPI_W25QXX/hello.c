#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
    printk(KERN_INFO "Hello Kernel!\n");
    return 0;
}

static void __exit my_exit(void)
{
    printk(KERN_INFO "Goodbye Kernel!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prahadeeshwaran");
MODULE_DESCRIPTION("Simple Hello World Kernel Module");