ifneq ($(KERNELRELEASE),) 
obj-m := proclog.o
else 
KERNELDIR ?= /lib/modules/$(shell uname -r)/build 

PWD := $(shell pwd)

default: 
	$(info Hit default)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
  
endif 

clean:
	$(info Hit clean)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

test:
	$(info Hit test)
  	# We put a — in front of the rmmod command to tell make to ignore
  	# an error in case the module isn’t loaded.
	-sudo rmmod proclog
  	# Clear the kernel log without echo
	sudo dmesg -C
  	# Insert the module
	sudo insmod proclog.ko
  	# Display the kernel log
	dmesg