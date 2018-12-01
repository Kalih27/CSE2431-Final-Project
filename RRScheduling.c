// C for implementation of RR scheduling 
#include <stdio.h>

// Function to calculate turnaround time 
void findTurnAroundTime(int processes[], int n, int bt[], int wt[], int tat[]){ 
    	for (int i = 0; i < n ; i++){ 
        	tat[i] = bt[i] + wt[i]; 
	}
} 

// Function to find waiting time for all processes 
void findWaitingTime(int processes[], int n, int bt[], int wt[], int quantum) 
{ 
	// Copy burst times bt[] 
	int remain_bt[n]; 
	for (int i = 0 ; i < n ; i++){
		remain_bt[i] = bt[i]; 
	}

	int t = 0;

	// RR scheduling
	while (1) { 
		int done = 1; 
		for (int i = 0 ; i < n; i++){ 
			if (remain_bt[i] > 0){ 
				done = 0;
				if (remain_bt[i] > quantum){ 
					t += quantum; 
					remain_bt[i] -= quantum; 
				} 
				else{ 
					t += remain_bt[i]; 
					wt[i] = t - bt[i]; 
					remain_bt[i] = 0; 
				} 
			} 
		} 
		// When all processes are done
		if (done) 
		break; 
	} 
} 

// Function to calculate average time 
void findavgTime(int processes[], int n, int bt[], int quantum){ 
	int wt[n], tat[n], total_wt = 0, total_tat = 0; 

	// Function to find waiting time of all processes 
	findWaitingTime(processes, n, bt, wt, quantum);
	findTurnAroundTime(processes, n, bt, wt, tat); 

	// Display processes along with all details 
	printf("Processes  Burst Time  Waiting Time TurnAround Time\n"); 

	// Calculate total waiting time
	for (int i=0; i<n; i++) 
	{ 
		total_wt = total_wt + wt[i]; 
		total_tat = total_tat + tat[i]; 
		printf(" %d \t\t %d \t\t %d \t\t %d\n", i+1, bt[i], wt[i], tat[i]); 
	} 
	printf("Average waiting time = %f\n", (float)total_wt / (float)n );
	printf("Average turnaround time = %f\n", (float)total_tat / (float)n );
	
} 


int main() 
{ 
	int n, quantum, i;
	int burst_time[20], processes[20]; 

    	printf("Enter Time quantum:");
    	scanf("%d",&quantum);

    	printf("Enter number of process:");
    	scanf("%d",&n);

    	for(i = 0;i < n;i ++){
        	processes[i] = i+1;       
    	}
 
    	printf("\nEnter Burst Time:\n");
    	for(i = 0;i < n;i ++){
        	printf("p%d:", i+1);
        	scanf("%d", &burst_time[i]);     
    	}

	findavgTime(processes, n, burst_time, quantum); 

	return 0; 
} 

