KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex24_platform_dts_irq.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
