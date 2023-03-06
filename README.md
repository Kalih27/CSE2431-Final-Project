# CSE2431-Final-Project (Original)

## Group Members (Original):
### Jiaqian Huang, Andrew Maloney, Alec Wilson

## Files:
### Makefile, runLogger, logger.c, proclog.c

## Requirements:
1. Make sure you have installed:
	```sh
	$ sudo apt-get install build-essential
	```
      
## How to run Logger file:
1. Compile the logger.c file
   ```sh
   $ runLogger
   ``` 
2. Run the following code
   ```sh
   ./logger
   ```
3. In terminal, the user will be asked whether the process should be logged. If not, the output will be showed in the terminal directly.
4. If yes, the user will be asked for the rate at which the process will poll the proc folder. The user can choose the option from 20 to 999 ms.
5. Then the user needs to input the proc id for which the length of next CPU burst will be calculated. (The user can get this from the list of process ids in terminal with help of some command lines.)
6. Next the user will expect to receive a a log.txt file to store the actual length of next 10 CPU burst.
7. Eventually, the user can choose to continue executing the programming or quit. If not, the program will be terminated.
8. If yes, the above steps will be repeated.  

## How to use proclog.c (aka kernel module):
1. Make the proclog kernel module by following command.
   ```sh
   $ make
   ```
2. Install the kernel module by following command. Then a file /proc/timing_log can be written to by various programs.
   ```sh
   $ sudo insmod proclog.ko
   ```
3. You can verify this by running the following code, which lists all of the modules currently in the kernel. Among them, you should see proclog. Note that the kernel replaces dashes in your module’s filename with underscores when it loads it. If you want to remove it, you can run the following command. 
   ```sh
   lsmod
   ```
   ```sh
   sudo rmmod proclog
   ```
4. To see an example of this, run the following code:
   ```sh
   $ gcc -o test test.c
   ```
5. You will need sudo privaleges to write to a proc file.
   ```sh
   $ sudo ./test
   ``` 
6. Run the following code. It shows that the test file ran and printed "This is test output" to the proc file. Make sure that any writing done to the proc file ends with a new line character.
   ```sh
   $ cat /proc/timing_log
   ```

## Updates
1. This repository is forked by JH, one of original contributors of this repository.
2. JH will be the member to work on this forked version for further upgrades. 
   
