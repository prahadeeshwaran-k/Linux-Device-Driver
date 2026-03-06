# ProcFS Kernel Module (Linux Kernel 6.x)

## 1. Overview

This Linux kernel module demonstrates how to create a **directory and file inside the `/proc` filesystem** and handle **read and write operations** from user space.

The module creates:

```
/proc/hello/
/proc/hello/dummy
```

Users can interact with this file using standard Linux commands such as:

```
cat
echo
```

The module implements **callback functions** that are executed when a user reads or writes to the proc file.

---

# 2. What is ProcFS

`procfs` is a **virtual filesystem in Linux** that provides information about the **kernel and running processes**.

Key characteristics:

* Exists in memory (not on disk)
* Managed by the Linux kernel
* Used to expose kernel information to user space

Example:

```
/proc/cpuinfo
/proc/meminfo
/proc/modules
```

Kernel modules can also create **custom proc entries** for debugging or configuration.

---

# 3. Module Features

This module demonstrates:

* Creating a **proc directory**
* Creating a **proc file**
* Implementing **read callback**
* Implementing **write callback**
* Safe data transfer using:

  * `copy_to_user()`
  * `copy_from_user()`

---

# 4. Directory Structure

Example project layout:

```
procfs_module/
│
├── proc_example.c
├── Makefile
└── README.md
```

---

# 5. Kernel Interfaces Used

| Kernel API         | Purpose                          |
| ------------------ | -------------------------------- |
| `proc_mkdir()`     | Creates directory inside `/proc` |
| `proc_create()`    | Creates proc file                |
| `proc_remove()`    | Removes proc entry               |
| `copy_to_user()`   | Send data from kernel to user    |
| `copy_from_user()` | Receive data from user           |
| `proc_ops`         | Defines read/write callbacks     |

---

# 6. How the Module Works

## Step 1: Module Loading

When the module loads, the function:

```
my_init()
```

is executed.

It performs the following actions:

1. Creates directory

```
/proc/hello
```

2. Creates file

```
/proc/hello/dummy
```

3. Registers read and write callbacks.

---

## Step 2: Read Operation

When a user executes:

```
cat /proc/hello/dummy
```

The kernel calls:

```
my_read()
```

The function sends a message to user space:

```
Hello from a procfs file!
```

---

## Step 3: Write Operation

When a user executes:

```
echo "hello kernel" > /proc/hello/dummy
```

The kernel calls:

```
my_write()
```

The data written by the user is copied into kernel space and printed in the kernel log.

---

# 7. Data Flow

```
User Space
   │
   ├── cat /proc/hello/dummy
   │         │
   │         └── my_read()
   │
   └── echo "text" > /proc/hello/dummy
             │
             └── my_write()

Kernel Space
```

---

# 8. Makefile

Create a file named **Makefile**.

```
obj-m += proc_example.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

---

# 9. Compilation

Run the following command:

```
make
```

Output:

```
proc_example.ko
```

This is the **kernel module**.

---

# 10. Loading the Module

Insert the module into the kernel:

```
sudo insmod proc_example.ko
```

Verify:

```
lsmod | grep proc_example
```

Check kernel log:

```
dmesg
```

---

# 11. Checking Proc Entry

Verify the directory:

```
ls /proc/hello
```

Output:

```
dummy
```

---

# 12. Reading the Proc File

Run:

```
cat /proc/hello/dummy
```

Output:

```
Hello from a procfs file!
```

---

# 13. Writing to the Proc File

Run:

```
echo "Test Message" > /proc/hello/dummy
```

Check kernel log:

```
dmesg
```

Example output:

```
procfs: user wrote: Test Message
```

---

# 14. Removing the Module

Unload the module:

```
sudo rmmod proc_example
```

Verify removal:

```
ls /proc/hello
```

The directory will be removed.

---

# 15. Important Kernel Concepts

### 1. `copy_to_user()`

Copies data from **kernel space → user space**.

```
copy_to_user(user_buffer, kernel_buffer, size);
```

---

### 2. `copy_from_user()`

Copies data from **user space → kernel space**.

```
copy_from_user(kernel_buffer, user_buffer, size);
```

---

### 3. `proc_ops`

Structure used to define **file operations for procfs**.

Example:

```
static const struct proc_ops fops = {
    .proc_read  = my_read,
    .proc_write = my_write,
};
```

---

# 16. Kernel Version Compatibility

This module is compatible with:

```
Linux Kernel ≥ 5.6
Linux Kernel 6.x
```

Because it uses:

```
struct proc_ops
```

Older kernels used:

```
struct file_operations
```

---

# 17. Safety Considerations

Important safety checks implemented:

* Prevent buffer overflow
* Validate user memory access
* Use kernel-safe copy functions

Never directly access user-space pointers.

Always use:

```
copy_to_user()
copy_from_user()
```

---

# 18. Use Cases of ProcFS in Drivers

ProcFS is commonly used for:

* Driver debugging
* Kernel configuration
* Runtime statistics
* Hardware diagnostics
* Performance monitoring

Example drivers using procfs:

* Network drivers
* Filesystem drivers
* Embedded platform drivers

---

# 19. Example Output

After loading the module:

```
/proc
 └── hello
      └── dummy
```

Reading:

```
$ cat /proc/hello/dummy
Hello from a procfs file!
```

Writing:

```
$ echo "Hello Kernel" > /proc/hello/dummy
```

Kernel log:

```
procfs: user wrote: Hello Kernel
```

---

# 20. References

Linux Kernel Documentation:

```
https://docs.kernel.org/filesystems/proc.html
```

Books:

* Linux Device Drivers
* Linux Kernel Development
* Understanding the Linux Kernel

---

# 21. Author

Author: Johannes 4 GNU/Linux
Kernel Version: Linux 6.x
Module Type: ProcFS Example
