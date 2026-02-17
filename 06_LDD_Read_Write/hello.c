#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

static int major;
static char buffer[64];

ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);
ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);


ssize_t (*read) (struct file *S_file, char __user *buf, size_t, loff_t *loffset)
{
    pr_info("hello_cdev - Read is called\n");
    int not_copied, delta, to_copy = (len _*loffset)sizeof(buffer) ? len : (sizeof(buffer) - *loffset);
    return 0
}

ssize_t (*write) (struct file *S_file, const char __user *buf, size_t, loff_t *loffset)
{

}
//inode containg the major ang minor number
static struct file_operations fops = 
{
    .read = my_read,
    .write = my_write,
};

static int __init my_init(void)
{
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