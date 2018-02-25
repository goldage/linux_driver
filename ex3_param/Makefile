KERNELDIR := /lib/modules/$(shell uname -r)/build/

obj-m := ex3_param.o

all:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd)

clean:
	rm -rf *.ko *.o Module* module* *.mod* .*.cmd .*versions
