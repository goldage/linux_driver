KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex9_private_data.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
