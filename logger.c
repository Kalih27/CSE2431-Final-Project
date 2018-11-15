//CSE 2431
//Andrew Maloney, Alec Wilson, Jiaqian Huang

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>

//used to calculate next burst time
#define ALPHA 0.5

//flag for alarm() command
//1 when alarm goes off, 0 otherwise
volatile int alarmFlag = 0;

//function header
long int nextBurstTime(long int, double, long int);

//alarm() handler
//changes alarmFlag to 1
void alarmHandler(int sig){
	alarmFlag = 1;
}

int main() {
	//file pointer for log file and read file
	FILE *logFile, *readFile;
	
	//pid of process to monitor
	int neededPID=1;

	//string to hold full file path
	char filePath[100];

	//longs to hold utimes and stimes (both are clock ticks)
	// utime: CPU time measured in user code, measured in clock ticks
	// stime: CPU time measured in kernel code, measured in clock ticks
	long utime, stime;
	long lutime, lstime;
	utime=stime=lutime=lstime=0;

	//longs to hold delta utime stime
	long dutime, dstime;

	//long to hold predicted next clock ticks
	long taunu=10, tauns=10;
	long ltaunu=10, ltauns=10;
	
	//get pid from user
	printf("Please enter PID of process to measure: ");
	scanf("%d", &neededPID);
	//puts("s\n");

	//create string for file path
	sprintf(filePath, "/proc/%d/stat", neededPID);
	
	//print file path before trying to open file
	printf("Reading from \"%s\"\n", filePath);
	
	//puts("l");
	//open file to log data too
	logFile = fopen("log.txt", "w");
	if(logFile == NULL){
		printf("Could not open log file\n");
		return 0;
	}

	// write column title to the log.txt
	fprintf(logFile, "utime stime\n");

	//signal so that we can use alarm
	signal(SIGALRM, alarmHandler);

	//puts("a");
	//initialize alarmFlag to 0
	alarmFlag = 0;
	//puts("o");
	int outerLoop = 1, inner = 0; //loop variable
	
	alarm(1);

	while(outerLoop){
		//if alarm goes off
		if(alarmFlag){
			printf("Alarm happened!\n");

			//open /proc/<neededPID> folder
			readFile = fopen(filePath, "r");
			if(readFile == NULL){
				printf("Could not open read file\n");
				return 0;
			}

			//read utume and stime from readFile aka the scariest fscanf in the west
			fscanf(readFile,"%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu", &utime, &stime);

			//offset to beginning of file
			//rewind(readFile);
			fclose(readFile);

			//write number to log file
			fprintf(logFile, "%lu %lu\n", utime, stime);

			printf("User Ticks: %lu\tSystem Ticks: %lu\n", utime, stime);

			//Print difference if applicable
			if(inner>0){
				dutime = utime-lutime;
				dstime = stime-lstime;
				taunu = nextBurstTime(dutime, ALPHA, ltaunu);
				tauns = nextBurstTime(dstime, ALPHA, ltauns);
				printf("User delta: %lu\tSystem delta: %lu\n", dutime, dstime);
				printf("Next User: %lu\tNext System: %lu\n\n", taunu, tauns);

			}

			//Set last tick count
			lutime=utime;
			lstime=stime;
			ltaunu=taunu;
			ltauns=tauns;

			//increment inner
		  	inner++;

			//if inner = 10 (ten 1-second waits) exit loop
		  	if(inner==10){
			  outerLoop=0;
			  printf("Leaving loop!\n");
			}

			//reset alarm flag
			alarmFlag = 0;
			alarm(1);
		}
	}


	
	printf("Bye!\n");
	//close file
	fclose(logFile);
	//fclose(readFile);
}


// Exponential Equation to get the next burst time
long int nextBurstTime(long int tn, double alpha, long int taun){
	long int nextBurst; 
	nextBurst = alpha * tn + (1 - alpha) * taun;
	return nextBurst;
}
