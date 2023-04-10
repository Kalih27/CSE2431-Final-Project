ifneq ($(KERNELRELEASE),) 
obj-m := proclog.o
else 
KERNELDIR ?= /lib/modules/$(shell uname -r)/build 

PWD := $(shell pwd)

default: 
	$(info Hit default)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
	gcc -o export_log_file export_log_file.c -lpthread
  
endif 

clean:
	$(info Hit clean)
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean

exec:
	$(info Hit exec)
  	# We put a — in front of the rmmod command to tell make to ignore
  	# an error in case the module isn’t loaded.
	-sudo rmmod proclog
  	# Clear the kernel log without echo
	# sudo dmesg -C
  	# Insert the module
	sudo insmod proclog.ko
  	# Display the kernel log
	dmesg

stop:
	$(info Hit stop)
  	# We put a — in front of the rmmod command to tell make to ignore
  	# an error in case the module isn’t loaded.
	-sudo rmmod proclog
	sudo dmesg -C