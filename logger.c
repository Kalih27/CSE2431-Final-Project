//CSE 2431
//Andrew Maloney
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main() {
	//file pointer for log file and read file
	FILE *logFile, *readFile;
	
	//pid of process to monitor
	int neededPID;

	//string to hold full file path
	char *filePath;
	
	//get pid from user
	printf("Please enter PID of process to measure: ");
	scanf("%d", &neededPID);
	
	//create string for file path
	sprintf(filePath, "/proc/%d/stat", neededPID);
	
	//print file path before trying to open file
	printf("%s\n", filePath);

	//open /proc/<neededPID> folder
	//readFile = fopen(filePath, r);
}
