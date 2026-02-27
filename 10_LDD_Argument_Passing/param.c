/******************************************************************************
 * param.c
 *
 * Simple Linux Kernel Module demonstrating:
 * 1. Integer parameter
 * 2. String parameter
 * 3. Array parameter
 * 4. Callback parameter
 *
 ******************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

/*----------------------------------------------------------
 * 1. PARAMETERS (variables configurable during insmod)
 *---------------------------------------------------------*/

// integer parameter
int my_value = 0;

// string parameter
char *my_name = "default";

// integer array parameter
int my_array[4] = {0};

// callback parameter
int my_cb_value = 0;


/*----------------------------------------------------------
 * 2. NORMAL PARAMETERS
 *---------------------------------------------------------*/

module_param(my_value, int, 0644);
MODULE_PARM_DESC(my_value, "An integer value");

module_param(my_name, charp, 0644);
MODULE_PARM_DESC(my_name, "A string value");

module_param_array(my_array, int, NULL, 0644);
MODULE_PARM_DESC(my_array, "Array of integers");


/*----------------------------------------------------------
 * 3. CALLBACK PARAMETER
 *   Called whenever value changes
 *---------------------------------------------------------*/

// custom setter function
static int param_callback(const char *val,
                          const struct kernel_param *kp)
{
    int ret;

    // store value using kernel helper
    ret = param_set_int(val, kp);

    if (ret == 0)
        printk(KERN_INFO "Callback triggered! New value = %d\n",
               my_cb_value);

    return ret;
}

/* operations structure */
static const struct kernel_param_ops cb_ops = {
    .set = param_callback,   // custom write
    .get = param_get_int,    // default read
};

/* register callback parameter */
module_param_cb(my_cb_value, &cb_ops, &my_cb_value, 0644);


/*----------------------------------------------------------
 * 4. MODULE INIT FUNCTION
 *   Runs when module loads
 *---------------------------------------------------------*/
static int __init hello_init(void)
{
    int i;

    printk(KERN_INFO "\n=== Module Loaded ===\n");

    printk(KERN_INFO "Integer value : %d\n", my_value);
    printk(KERN_INFO "Callback value: %d\n", my_cb_value);
    printk(KERN_INFO "Name          : %s\n", my_name);

    for (i = 0; i < 4; i++)
        printk(KERN_INFO "Array[%d] = %d\n", i, my_array[i]);

    return 0;
}


/*----------------------------------------------------------
 * 5. MODULE EXIT FUNCTION
 *---------------------------------------------------------*/
static void __exit hello_exit(void)
{
    printk(KERN_INFO "=== Module Removed ===\n");
}


/* Register init & exit */
module_init(hello_init);
module_exit(hello_exit);


/*----------------------------------------------------------
 * 6. MODULE INFORMATION
 *---------------------------------------------------------*/
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Learning Version");
MODULE_DESCRIPTION("Simple Module Parameter Example");
MODULE_VERSION("1.0");