#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

int my_value = 0;

/*---------------- CALLBACK FUNCTION ----------------*/
static int param_callback(const char *val, const struct kernel_param *kp)
{
    int ret;

    // kernel helper: string → int → store
    ret = param_set_int(val, kp);

    if (ret == 0)
        printk(KERN_INFO "Callback triggered! New value = %d\n", my_value);

    return ret;
}

/* parameter operations */
static const struct kernel_param_ops cb_ops = {
    .set = param_callback,
    .get = param_get_int,
};

/* register parameter WITH callback */
module_param_cb(my_value, &cb_ops, &my_value, 0644);
MODULE_PARM_DESC(my_value, "Integer with callback");

/*---------------- INIT ----------------*/
static int __init hello_init(void)
{
    printk(KERN_INFO "\n=== Module Loaded ===\n");
    printk(KERN_INFO "Integer value : %d\n", my_value);
    return 0;
}

/*---------------- EXIT ----------------*/
static void __exit hello_exit(void)
{
    printk(KERN_INFO "=== Module Removed ===\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");