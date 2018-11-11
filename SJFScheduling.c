// C for implementation of SJF scheduling 
#include<stdio.h>

// Function to calculate turnaround time 
void findTurnAroundTime(int wt[], int bt[], int n, int p[]){
	int total = 0, tat[20], i, j;
	float avg_tat;
	for(i = 0;i < n;i ++) {
        	tat[i] = bt[i] + wt[i]; 
        	total += tat[i];
        	printf("\np%d\t\t  %d\t\t    %d\t\t\t%d", p[i], bt[i], wt[i], tat[i]);
    	}
	
	avg_tat = (float)total/n;  
 	printf("\n\nAverage Turnaround Time=%f\n", avg_tat);
}

// Function to find waiting time for all processes 
void findWaitingTime(int bt[], int n, int wt[], int p[]){
	wt[0]=0;     
    	float avg_wt;  
	int i, j, total=0; 

    	for(i = 1;i < n;i ++){
        	wt[i]=0;
        	for(j = 0;j < i;j ++){
            		wt[i] += bt[j];
		}
 		total+=wt[i];
    	}
 
    	avg_wt=(float)total/n;  
 
    	printf("\nProcess\t    Burst Time    \tWaiting Time\t Turnaround Time");
    	
	findTurnAroundTime(wt, bt, n, p);
	
    	printf("\nAverage Waiting Time=%f\n", avg_wt);
}


void main(){

	int bt[20],p[20],wt[20],i,j,n,pos,temp;

    	printf("Enter number of process:");
    	scanf("%d",&n);
 
    	printf("\nEnter Burst Time:\n");
    	for(i = 0;i < n;i ++){
        	printf("p%d:", i+1);
        	scanf("%d", &bt[i]);
        	p[i] = i+1;       
    	}
 
    	// Sorting burst time from low to high
    	for(i = 0; i < n; i ++){
        	pos = i;
        	for(j = i+1; j < n; j ++){
            		if(bt[j] < bt[pos]){
                		pos = j;
			}
        	}
 
        	temp = bt[i];
        	bt[i] = bt[pos];
        	bt[pos] = temp;
 
        	temp = p[i];
        	p[i] = p[pos];
        	p[pos] = temp;
    	}
 
 	findWaitingTime(bt, n, wt, p);
}
