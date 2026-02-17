#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major;
ssize_t my_read (struct file *f, char __user *u, size_t l, loff_t *o);
ssize_t my_read (struct file *f, char __user *u, size_t l, loff_t *o)
{
    printk("hello_cdev - Read is Called\n");
    return 0;
}

static struct file_operations fops = 
{
    .read = my_read
};

static int __init my_init(void)
{
    //create a charDriver 1 major and in the range o 0 to 255 minor device
    //What register_chrdev() Actually Does int register_chrdev(unsigned int major, const char *name, const struct file_operations *fops);
    /*  It:
        Registers a character device driver
        Associates it with a major number
        Links it with your file_operations
        Adds entry inside /proc/devices
    */
    
    /* It does NOT:
    ❌ Create /dev/your_device
    ❌ Create sysfs entry
    ❌ Create device node automatically
    */
    major = register_chrdev(0,"hello_cdev",&fops ); 
    if(major <0) 
    {
        printk("hello_cdev - error Registering ChrDev\n");
        return 0;
    }
    printk(KERN_INFO "Major Device Number number is %d\n", major);
    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev(0,"hello_cdev" );
    printk(KERN_INFO "Goodbye Kernel!\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prahadeeshwaran");
MODULE_DESCRIPTION("Simple Hello World Kernel Module");