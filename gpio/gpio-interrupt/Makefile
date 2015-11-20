obj-m := gpio_interrupt.o

export ARCH=arm
export CROSS_COMPILE=arm-linux-gnueabihf-

SRC := $(shell pwd)

KERNEL_SRC =  /home/rxhc/work/socfpga/linux-socfpga

all:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules

modules_install:
	$(MAKE) -C $(KERNEL_SRC) M=$(SRC) modules_install

clean:
	rm -f *.o *~ core .depend .*.cmd *.ko *.mod.c
	rm -f Module.markers Module.symvers modules.order
	rm -rf .tmp_versions Modules.symvers
