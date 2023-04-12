




static void export_virtual_file_into_actual_file(void)
{
	// Allocate a buffer to read data from the virtual file
	char *buffer = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!buffer)
	{
		pr_err("Failed to allocate memory for buffer\n");
	}

	// Create the actual file on disk
	actual_file = filp_open(ACTUAL_FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (IS_ERR(actual_file))
	{
		pr_err("Failed to create actual file\n");
	}

	if (buffer)
	{
		kfree(buffer);
	}
	if (virtual_file)
	{
		filp_close(virtual_file, NULL);
	}
	if (actual_file)
	{
		filp_close(actual_file, NULL);
	}
}

	export_virtual_file_into_actual_file();
    
    // ktime_t current_time = ktime_get();
	// s64 current_time_ns = ktime_to_ns(current_time);
	// long current_time_s = current_time_ns / 1000000000;
	// start_time_s = current_time;
    	// unsigned long long cutime, cstime, start_time;	
    
    
    // unsigned long long utime_sec, stime_sec, start_time_sec;
	// unsigned long long utime_msec, stime_msec, start_time_msec;
	// unsigned long long total_time;
	// long long cpu_usage = 0;
	// long long elapsed_nsec;
	// long long usage_nsec;
    	// long long usage_sec;
        	// unsigned long long utime, stime, cutime, cstime;	
    
    // static int clk_tck = 100;
// static int number_of_cpu = 2;


// // pointer for buffer location in read
// static char *buff_ptr;



    // // Copy the virtual file's contents to the buffer
	// ssize_t ret = kernel_read(virtual_file, buffer, PAGE_SIZE, &virtual_file->f_pos);

	// Read data from the virtual file and write it to the actual file on disk
	// while ((bytes_read = kernel_read(virtual_file, buffer, PAGE_SIZE, &virtual_file->f_pos)) > 0)
	// {
	//     ssize_t bytes_written = kernel_write(actual_file, buffer, bytes_read, &actual_file->f_pos);
	//     if (bytes_written != bytes_read)
	//     {
	//         pr_err("Failed to write data to %s\n", ACTUAL_FILE_PATH);
	//     }
	// }
    	// // Open the virtual file
	// virtual_file = filp_open(PROC_FILE_PATH, O_RDONLY, 0);
	// if (IS_ERR(virtual_file))
	// {
	// 	pr_err("Failed to open virtual file\n");
	// }

	// char *temp = (char *)v;
	// do
	// {
	// 	seq_putc(s, *temp);
	// 	temp++;
	// } while (*temp != '\n');
	// seq_putc(s, '\n');


    
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
void main()
{
    int n, i, j, k, temp = 65, flag = 0;
    char process[20];
    int brust[20], priority[20], pos;
    int time = 0, quantom = 1, tbt = 0;
    int z = 0, lottery[20], ticket[20][20], q = 0;
    printf("Enter Number Of Process: ");
    scanf("%d", &n);
    if (n <= 0)
    {
        printf("\n\n::::  Invalid Value Of Number Of Process  ::::");
        exit(0);
    }
    for (i = 0; i < n; i++)
    {
        process[i] = temp;
        temp += 1;
    }
    for (i = 0; i < n; i++)
    {
        printf("\nEnter The Brust Time For Process %c: ", process[i]);
        scanf("%d", &brust[i]);
        printf("Enter The Priority For Process %c(b/w 1 to %d): ", process[i], n);
        scanf("%d", &priority[i]);
    }

    // sorting burst time, priority and process number in ascending order using selection sort
    for (i = 0; i < n; i++)
    {
        pos = i;
        for (j = i + 1; j < n; j++)
        {
            if (priority[j] < priority[pos])
                pos = j;
        }

        temp = process[i];
        process[i] = process[pos];
        process[pos] = temp;

        temp = brust[i];
        brust[i] = brust[pos];
        brust[pos] = temp;

        temp = priority[i];
        priority[i] = priority[pos];
        priority[pos] = temp;

        if (brust[i] < 0)
        {
            flag = 1;
        }
    }

    if (flag == 1)
    {
        printf("\n\n:::  Invalid Time Entered ::: \n");
        exit(0);
    }

    printf("\n |Priority |  Process   |  Brust |");

    for (i = 0; i < n; i++)
    {
        printf("\n |   %d \t   |    %c    \t|   %d    |", priority[i], process[i], brust[i]);
        tbt = tbt + brust[i];
    }

    printf("\n\n:::::::::   Quantom time is 1 Sec   ::::::::: \n\n");

    // assign one or more lottery numbers to each process
    int p = 1, m_ticket = 0;
    printf("\n\n\nPriority  process  Brust  Lottery  Tickets");
    for (i = 0; i < n; i++)
    {
        lottery[i] = (brust[i] / quantom) + (n - priority[i]);
        for (z = 0; z < lottery[i]; z++)
        {
            ticket[i][z] = p++;
            m_ticket = p;
        }

        printf("\n  %d\t    %c\t    %d \t    %d\t", priority[i], process[i], brust[i], lottery[i]);
        for (z = 0; z < lottery[i]; z++)
        {
            if (ticket[i][z] < 10)
                printf("  ::%d:: ", ticket[i][z]);
            else
                printf("  ::%d::", ticket[i][z]);
        }
    }

    while (time != tbt)
    {

        int winner = (rand() % m_ticket - 1) + 1;
        for (i = 0; i < n; i++)
            for (z = 0; z < lottery[i]; z++)
                if (ticket[i][z] == winner)
                    q = i;

        printf("\n\n-------------------------------------");
        printf("<<<<   Winner: %d   >>>>", winner);
        printf("-------------------------------------\n");

        if ((brust[q] > 0))
        {
            brust[q] -= quantom;

            if (brust[q] > 0)
            {
                time += quantom;
            }
            else
            {
                time += (brust[q] + quantom);
            }

            if (brust[q] < 0)
            {
                brust[q] = 0;
            }
            printf("\n\t\t\t\t  Process That Are Running Is: %c", process[q]);
            printf("\n\t   (Total Time << Remaining Brust Time Of This Process << process ):  ( %d  <<  %d  <<  %c )\n", time, brust[q], process[q]);
        }
        else
        {
            printf("\n\t\t     >>>>>>Related Process With This Ticket Has Been Completed<<<<<<\n");
        }
    }
}