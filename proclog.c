/********* proclog.c ***********/
// Logger that creates a proc file
// idea from tldp.org/LDP/lkmpg/2.6/html/index.html
#include "headers.h"
#include "process.c"

#ifndef __KERNEL__
#define __KERNEL__
#endif

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Kernel module to log process times");

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 6, 0)
#define HAVE_PROC_OPS
#endif

static void *proc_seq_start(struct seq_file *s, loff_t *pos)
{
	printk("Hit proc_seq_start");

	static unsigned long counter = 0;

	/* beginning a new sequence ? */
	if (*pos == 0)
	{
		/* yes => return a non null value to begin the sequence */
		return &counter;
	}
	else
	{
		/* no => it's the end of the sequence, return end to stop reading */
		*pos = 0;
		return NULL;
	}
}

static void *proc_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	printk("Hit proc_seq_next");

	char *temp = (char *)v;
	temp++;
	printk("Temp increased.");
	(*pos)++;
	printk("Position increased.");
	printk("Position is %Ld\n", (*pos));
	return NULL;
}

static void proc_seq_stop(struct seq_file *s, void *v)
{
	printk("Hit proc_seq_stop");
}

static long get_process_elapsed_time(struct task_struct *task)
{
	// adapted from https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat
	// /proc/[PID]/stat
	// #14 utime - CPU time spent in user code, measured in clock ticks
	// #15 stime - CPU time spent in kernel code, measured in clock ticks
	// #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
	// #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
	// #22 starttime - Time when the process started, measured in clock ticks
	unsigned long long utime, stime, cutime, cstime, start_time;
	unsigned long long utime_sec, stime_sec, start_time_sec;
	unsigned long long utime_msec, stime_msec, start_time_msec;
	unsigned long long total_time;
	long long cpu_usage = 0;
	long long elapsed_nsec, usage_nsec;
	long long elapsed_sec, usage_sec;
	int clk_tck = 100; // constants
	int number_of_cpu = 2;

	if (task == NULL)
	{
		return -EINVAL;
	}

	// The reason for this is that the utime value in the /proc/[pid]/stat file is measured in clock ticks,
	// whereas the utime field in the task_struct is measured in nanoseconds.
	start_time = task->start_time;

	// kernel system timer
	uptime = ktime_divns(ktime_get_coarse_boottime(), NSEC_PER_SEC);

	elapsed_sec = (long)uptime * 1000000000 - start_time;

	return elapsed_sec;
}

static int proc_seq_show(struct seq_file *s, void *v)
{
	printk("Hit proc_seq_show");

	loff_t *spos = (loff_t *)v;

	unsigned long long utime, stime, cutime, cstime, start_time;
	unsigned long long total_time;
	struct task_struct *task;

	seq_printf(s,
			   "PID\t NAME\t ELAPSED_TIME\t TOTAL_TIME\t utime\t stime\t start_time\t uptime\t\n");
	for_each_process(task)
	{
		printk(KERN_INFO "Process: %s (pid: %d)\n", task->comm, task->pid);

		utime = task->utime;
		stime = task->stime;

		total_time = utime + stime;
		/* Get CPU usage for the process */
		long elapsed_time = get_process_elapsed_time(task);

		seq_printf(s,
				   "%d\t %s\t %lld\t %lld\t %lld\t %lld\t %lld\t %lld\t\n ",
				   task->pid,
				   task->comm,
				   elapsed_time,
				   total_time,
				   task->utime,
				   task->stime,
				   task->start_time,
				   ktime_divns(ktime_get_coarse_boottime(), NSEC_PER_SEC));
	}

	seq_printf(s, "%Ld\n", *spos);

	// char *temp = (char *)v;
	// do
	// {
	// 	seq_putc(s, *temp);
	// 	temp++;
	// } while (*temp != '\n');
	// seq_putc(s, '\n');

	return 0;
}

static struct seq_operations proc_seq_ops = {
	.start = proc_seq_start,
	.next = proc_seq_next,
	.stop = proc_seq_stop,
	.show = proc_seq_show};

static int procfile_single_open(struct inode *inode, struct file *file)
{
	printk("Hit procfile_single_open");
	return single_open(file, proc_seq_show, NULL);
}

static int procfile_open(struct inode *inode, struct file *file)
{
	printk("Hit procfile_open");
	return seq_open(file, &proc_seq_ops);
}

static ssize_t procfile_write(struct file *file, const char *buffer, size_t count, loff_t *off)
{
	printk("Hit procfile_write");
	return 1;
}

static int procfile_show(struct seq_file *m, void *v)
{
	printk("Hit procfile_show");
	return 0;
}

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
	.proc_open = procfile_open, // need continuous seq_open to make export function in c file work
	.proc_write = procfile_write,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release};
#else
static const struct file_operations proc_file_fops = {
	.owner = THIS_MODULE,
	.open = procfile_open, // need continuous seq_open to make export function in c file work
	.write = procfile_write,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release};
#endif

static int __init init_kernel_module(void)
{
	printk(KERN_INFO "Process logger module loaded\n");

	// initialize: 1. struct to hold info about proc file 2. other variables
	struct proc_dir_entry *log_file;
	endflag = 0;

	// adapted from stackoverflow.com/questions/8516021/proc-create-example-for-kernel-module
	// fixed the version issue from https://stackoverflow.com/questions/64931555/how-to-fix-error-passing-argument-4-of-proc-create-from-incompatible-pointer
	proc_create("log_file", 0, NULL, &proc_file_fops);

	return 0;
}

static void __exit exit_kernel_module(void)
{
	remove_proc_entry("log_file", NULL);
	printk(KERN_INFO "Process logger module unloaded\n");
}

module_init(init_kernel_module);
module_exit(exit_kernel_module);