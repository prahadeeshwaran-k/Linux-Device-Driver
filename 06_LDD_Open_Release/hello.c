#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major;

//inode containg the major ang minor number
static int my_open (struct inode * inode, struct file * file);
static int my_release (struct inode * inode, struct file *file);

static int my_open (struct inode *inode, struct file *file)
{
    printk("hello_cdev - Major: %d, Minor: %d\n ",imajor(inode),iminor(inode));
    pr_info("hello_cdev - file->f_pos: %lld\n", file->f_pos);
    pr_info("hello_cdev - file->f_mode: 0X%X\n", file->f_mode);
    pr_info("hello_cdev - file->f_flags: 0X%X\n", file->f_flags);
    return 0;
}
static int my_release (struct inode *inode, struct file *file)
{
    printk("hello_cdev - File is closed\n");
    return 0;
}
static struct file_operations fops = 
{
    .open = my_open,
    .release = my_release
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