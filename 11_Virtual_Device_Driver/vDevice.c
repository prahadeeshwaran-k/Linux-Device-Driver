#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>         // kmalloc()
#include <linux/uaccess.h>      // copy_to/from_user()
#include <linux/err.h>

#define mem_size 1024 

dev_t dev = 0;
static struct class *dev_class;
static struct cdev etx_cdev;      
uint8_t *kernel_buffer;           

static int      my_open(struct inode *inode, struct file *file);
static int      my_release(struct inode *inode, struct file *file);
static ssize_t  my_read(struct file *filp, char __user *buf, size_t len, loff_t * off);
static ssize_t  my_write(struct file *filp, const char __user *buf, size_t len, loff_t * off);

static struct file_operations fops =
{
    .owner          = THIS_MODULE,
    .read           = my_read,
    .write          = my_write,
    .open           = my_open,
    .release        = my_release,
};

static int my_open(struct inode *inode, struct file *file)
{
    pr_info("Device File Opened...!!!\n");
    return 0;
}

static int my_release(struct inode *inode, struct file *file)
{
    pr_info("Device File Closed...!!!\n");
    return 0;
}

static ssize_t my_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
    if( copy_to_user(buf, kernel_buffer, mem_size) )
    {
        pr_err("Data Read : Err!\n");
        return -EFAULT;
    }
    pr_info("Data Read : Done!\n");
    return mem_size;
}

static ssize_t my_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
    if( copy_from_user(kernel_buffer, buf, len) )
    {
        pr_err("Data Write : Err!\n");
        return -EFAULT;
    }
    pr_info("Data Write : Done!\n");
    return len;
}

/* Renamed from driver_init to etx_driver_init to avoid kernel name collision */
static int __init my_driver_init(void)
{
    if((alloc_chrdev_region(&dev, 0, 1, "Prahadeesh_Dev")) < 0){
        pr_err("Cannot allocate major number\n");
        return -1;
    }
    pr_info("Major = %d Minor = %d \n", MAJOR(dev), MINOR(dev));

    cdev_init(&etx_cdev, &fops);

    if((cdev_add(&etx_cdev, dev, 1)) < 0){
        pr_err("Cannot add the device to the system\n");
        goto r_class;
    }

    /* Kernel 6.4+ class_create only takes 1 argument */
    dev_class = class_create("Prahadeesh_class");
    if(IS_ERR(dev_class)){
        pr_err("Cannot create the struct class\n");
        goto r_cdev;
    }

    if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "Prahadeesh_device"))){
        pr_err("Cannot create the Device 1\n");
        goto r_device;
    }
    
    if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == NULL){
        pr_err("Cannot allocate memory in kernel\n");
        goto r_device;
    }
    
    strcpy(kernel_buffer, "Hello_World");
    
    pr_info("Device Driver Insert...Done!!!\n");
    return 0;

r_device:
    class_destroy(dev_class);
r_cdev:
    cdev_del(&etx_cdev);
r_class:
    unregister_chrdev_region(dev, 1);
    return -1;
}

/* Renamed from driver_exit to etx_driver_exit */
static void __exit my_driver_exit(void)
{
    kfree(kernel_buffer);
    device_destroy(dev_class, dev);
    class_destroy(dev_class);
    cdev_del(&etx_cdev);
    unregister_chrdev_region(dev, 1);
    pr_info("Device Driver Remove...Done!!!\n");
}

module_init(my_driver_init);
module_exit(my_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Prahadeesh");
MODULE_DESCRIPTION("A simple virtual character device driver");