# CSE2431-Final-Project
Git repository for final project

Requirements:
1. Make sure you have installed:
	$ sudo apt-get install build-essential
2. Use 
	$ make
 	to compile your kernel module into an .ko file. “ko” means kernel object.
3. Load your kernel module:
  	sudo insmod process.ko
4. After that, use 
	$ lsmod
	to see whether it is really loaded.
5. Unload your kernel module:
  	$ sudo rmmod module.ko
