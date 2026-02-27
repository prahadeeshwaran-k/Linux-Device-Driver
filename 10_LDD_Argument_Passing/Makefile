# Simple Kbuild Makefile for building one kernel module.
# This file tells Kbuild what to build and how to invoke the kernel build system.
#
# obj-m lists module objects to build. For each *.o here, Kbuild will:
#   - find the matching *.c file
#   - compile it into a module
#   - produce a *.ko output
# Example: hello.c -> hello.o -> hello.ko
obj-m += param.o

# Use the kernel build system from the currently running kernel.
# (You can override KDIR on the command line if needed.)
#M=$(PWD) tells the kernel build system “the module source lives in the current directory
#KDIR = /lib/moduels/$(shell uname -r)/build

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
