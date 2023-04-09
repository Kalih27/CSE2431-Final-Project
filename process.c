/********* process.c ***********/
#include "headers.h"

/*
    struct Process
        -burst_time: given, time it takes for the process to complete
        -next:      generated, next process in queue
        -previous:  generated, process before self in queue
*/
struct Process
{
    int burst_time;
    pid_t proc_pid;
    struct Process *next;
    struct Process *previous;
};

/*
    static int check_alloc()
        -checks if the given pointer has been allocated successfully and exits if it did not
*/
static int check_alloc(void *ptr)
{
    if (!ptr)
    {
        printk("Allocation error!\n");
        return 0;
    }
    return 1;
}

/*
    struct Process* create_new_process(int process_burst_time, pid_t pid)
        -creates a pointer to a Process struct
*/
struct Process *create_new_process(int process_burst_time, pid_t pid)
{
    struct Process *new_process = NULL;

    new_process = (struct Process *)kmalloc(sizeof(struct Process), GFP_KERNEL);
    if (check_alloc(new_process) == 0)
    {
        return -ENOMEM;
    }

    new_process->burst_time = process_burst_time;
    new_process->next = NULL;
    new_process->proc_pid = pid;
    new_process->previous = NULL;

    return new_process;
}