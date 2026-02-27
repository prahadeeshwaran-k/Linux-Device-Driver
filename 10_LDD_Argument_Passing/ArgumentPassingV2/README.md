# Linux Kernel Module – Parameter with Callback

## 📌 Overview

This project demonstrates how to use:

* `module_param()`
* `module_param_cb()`
* Custom parameter callback functions
* Runtime parameter modification using sysfs

The module allows changing a parameter both:

* During module load (`insmod`)
* After module load (`/sys/module/...`)

---

# 📂 File Structure

```
param.c
Makefile
README.md
```

---

# ⚙️ Build Instructions

## 1️⃣ Compile the Module

```bash
make
```

This generates:

```
param.ko
```

---

# 🚀 Load the Module

## Load with Initial Value

```bash
sudo insmod param.ko my_value=10
```

### Expected Kernel Log

```text
Callback triggered! New value = 10
=== Module Loaded ===
Integer value : 10
```

Check logs:

```bash
dmesg | tail
```

---

# 🔄 Change Parameter After Loading

The parameter can be modified at runtime using sysfs.

## Change Value

```bash
echo 20 | sudo tee /sys/module/param/parameters/my_value
```

### Expected Output (Kernel Log)

```text
Callback triggered! New value = 20
```

---

# 🔍 Verify Parameter

## List Parameters

```bash
ls /sys/module/param/parameters
```

Output:

```
my_value
```

## Read Current Value

```bash
cat /sys/module/param/parameters/my_value
```

---

# 🗑️ Remove the Module

```bash
sudo rmmod param
```

Expected log:

```text
=== Module Removed ===
```

---

# 🧠 How It Works

### During Module Load

```text
insmod param.ko my_value=10
        ↓
Kernel parses parameter
        ↓
Custom .set() callback runs
        ↓
init() function runs
```

---

### During Runtime Change

```text
echo 20 > my_value
        ↓
sysfs write
        ↓
.set() callback runs
        ↓
Value updated
```

---

# 🏗️ Key Concepts Demonstrated

| Feature             | Description                                    |
| ------------------- | ---------------------------------------------- |
| `module_param_cb()` | Registers parameter with custom handler        |
| `.set`              | Called when parameter is written               |
| `.get`              | Called when parameter is read                  |
| `param_set_int()`   | Kernel helper to safely convert string → int   |
| sysfs               | Auto-created interface under `/sys/module/...` |

---

# 📌 Important Notes

* Parameter name must match variable name exactly.
* Only root can modify parameters (permission 0644).
* Sysfs files are virtual (not real files on disk).
* Callback executes both during `insmod` and runtime changes.

---

# 🧪 Debug Tips

If parameter file does not exist:

```bash
lsmod | grep param
dmesg | tail
modinfo param.ko
```

Always rebuild after code changes:

```bash
make clean
make
```

---

# 🎯 Learning Outcome

After completing this example, you understand:

* How Linux kernel parses module parameters
* How to implement custom validation logic
* How sysfs connects userspace to kernel space
* How callbacks are triggered inside the kernel

---

If you want, I can also generate a **Version 2 README** that includes:

* Internal kernel call flow
* Memory explanation
* Interview questions section
* Advanced validation example

Just tell me 👍
