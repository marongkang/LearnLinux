ifeq ($(KERNELRELEASE),)
KDIR := /lib/modules/5.4.0-126-generic/build
PWD := $(shell pwd)
modules:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
modules_install:
	$(MAKE) -C $(KDIR) M=$(PWD) modules_install
clean:
	rm -rf *.o *.ko .depend *.mod.o *.mod.c Module.* modules.*
.PHONY:modules modules_install clean
else
	obj-m := my_blkdev.o
endif
