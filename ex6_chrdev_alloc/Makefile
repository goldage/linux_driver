KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex6_chrdev_alloc.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
