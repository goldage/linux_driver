KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex22_platform_driver.o
obj-m += ex22_platform_device.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
