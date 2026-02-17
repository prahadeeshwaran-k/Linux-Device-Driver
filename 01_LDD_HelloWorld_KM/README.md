# Hello World Kernel Module (Kbuild)

This folder contains a minimal Linux kernel module example built with Kbuild. It is meant as a learning starter: build the module, insert it into the kernel, and inspect the kernel log.

## Files

- `hello.c`: the module source that builds into `hello.ko`
- `makefile`: Kbuild Makefile that drives the build
- `main.c`: a scratch file (not used by the Makefile)

## Prerequisites

- Linux kernel headers for your running kernel
- A compiler toolchain (`gcc`, `make`)

On most distros, installing the kernel headers package is enough.

## Build

```bash
make
```

This produces `hello.ko` in the same directory.

## Clean

```bash
make clean
```

## Load and Unload

Load the module:

```bash
sudo insmod hello.ko
```

Unload the module:

```bash
sudo rmmod hello
```

## Check Output

Most hello modules print a message to the kernel log:

```bash
dmesg | tail -n 20
```

## Notes

- The module name comes from `obj-m += hello.o` in `makefile`.
- If you change the filename, update `obj-m` to match.
