KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex12_unlocked_ioctl_ioremap.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
