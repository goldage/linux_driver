KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex14_chrdev_open_atom.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
