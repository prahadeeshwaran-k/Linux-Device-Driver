#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/string.h>
#include <linux/kernel.h>

/* Module Information */
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ProcFS example for Linux kernel 6.x");

/* Proc directory and file */
static struct proc_dir_entry *proc_folder;
static struct proc_dir_entry *proc_file;

/* ================= READ FUNCTION ================= */

static ssize_t my_read(struct file *file,
                       char __user *user_buffer,
                       size_t count,
                       loff_t *offset)
{
    char text[] = "Hello from a procfs file!\n";
    int len = strlen(text);

    if (*offset >= len)
        return 0;

    if (copy_to_user(user_buffer, text, len))
        return -EFAULT;

    *offset = len;

    pr_info("procfs: read called\n");

    return len;
}

/* ================= WRITE FUNCTION ================= */

static ssize_t my_write(struct file *file,
                        const char __user *user_buffer,
                        size_t count,
                        loff_t *offset)
{
    char text[255];
    int to_copy;

    memset(text, 0, sizeof(text));

    to_copy = min(count, sizeof(text) - 1);

    if (copy_from_user(text, user_buffer, to_copy))
        return -EFAULT;

    pr_info("procfs: user wrote: %s\n", text);

    return to_copy;
}

/* ================= PROC OPERATIONS ================= */

static const struct proc_ops fops =
{
    .proc_read  = my_read,
    .proc_write = my_write,
};

/* ================= MODULE INIT ================= */

static int __init my_init(void)
{
    /* Create directory /proc/hello */
    proc_folder = proc_mkdir("prahadeesh_Proc", NULL);

    if (!proc_folder)
    {
        pr_err("procfs: cannot create /proc/prahadeesh_Proc\n");
        return -ENOMEM;
    }

    /* Create file /proc/hello/dummy */
    proc_file = proc_create("Nice", 0666, proc_folder, &fops);

    if (!proc_file)
    {
        pr_err("procfs: cannot create /proc/prahadeesh_Proc/Nice\n");
        proc_remove(proc_folder);
        return -ENOMEM;
    }

    pr_info("procfs: created /proc/prahadeesh_Proc/Nice\n");

    return 0;
}

/* ================= MODULE EXIT ================= */

static void __exit my_exit(void)
{
    pr_info("procfs: removing /proc/prahadeesh_Proc/Nice\n");

    proc_remove(proc_file);
    proc_remove(proc_folder);
}

module_init(my_init);
module_exit(my_exit);