obj-m := encriptador.o	# Module Name is encriptador.c

KDIR  := /lib/modules/$(shell uname -r)/build

all:
	$(MAKE) -C $(KDIR) M=$(shell pwd) modules

clean:
	$(MAKE) -C $(KDIR) M=$(shell pwd) clean
	$(RM) Module.markers modules.order
