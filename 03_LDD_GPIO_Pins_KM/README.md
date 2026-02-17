# Hello World Kernel Module (Kbuild)

This folder contains a minimal Linux kernel module example built with Kbuild. It is meant as a learning starter: build the module, insert it into the kernel, and inspect the kernel log.

## Files

- `hello.c`: the module source that builds into `hello.ko`
- `makefile`: Kbuild Makefile that drives the build
- `main.c`: a scratch file (not used by the Makefile)



## Gpio Status

```bash
gpiodetect
```
cd /sys/class/gpio/


