KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex17_chrdev_open_kmalloc.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
