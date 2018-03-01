KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex26_platform_dts_workqueue.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
