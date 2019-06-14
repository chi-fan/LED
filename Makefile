ifneq  ($(KERNELRELEASE),)
obj-m:=device.o driver.o
$(info "2nd")
else
ARCH := arm

KDIR := /files/linux-3.14.38/
#KDIR := /lib/modules/3.14.38-6UL_ga-00003-g9b1d9f6/	
PWD:=$(shell pwd)
all:
	$(info "1st")
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.symvers *.mod.c *.mod.o *.order
endif
