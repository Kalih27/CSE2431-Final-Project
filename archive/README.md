# CSE2431-Final-Project

## Group Members:
### Jiaqian Huang
### Andrew Maloney
### Alec Wilson

## Files
### Makefile, runLogger, logger.c, proclog.c

## Requirements:
1. Make sure you have installed:
	$ sudo apt-get install build-essential
      
## How to run Logger file
1. $ runLogger
    This will help compile the logger.c file and then run ./logger
2. You will be asked whether the process should be logged. If not the output will be showed in the terminal directly.
2. Will be asked for the rate at which the process will poll the proc folder (20 to 999 ms).
3. Input the proc id for which the length of next CPU burst will be calculated (You can get this from the list of process ids in terminal with help of some command lines.).
4. Get a log.txt file to store the actual length of next 10 CPU burst.
5. Choose to continue executing the programming or quit

## How to use proclog.c
1. $ make
    This will make the proclog kernel module
2. $ sudo insmod proclog.ko
    This will install the kernel module. A file /proc/timing_log can now be written to by various programs
3. To see an example of this run the following code:
4. $ gcc -o test test.c
5. $ sudo ./test
    You will need sudo privaleges to write to a proc file
6. $ cat /proc/timing_log
    This shows that the test file ran andd printed "This is test output" to the proc file.
    Make sure that any writing done to the proc file ends with a new line character