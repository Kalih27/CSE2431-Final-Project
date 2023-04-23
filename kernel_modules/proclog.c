/********* proclog.c ***********/
// Logger that creates a proc file
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h> // seq_read, ...
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/jiffies.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/sched/signal.h>
#include <linux/pid_namespace.h>
#include <asm/io.h>
#include <linux/tick.h>
#include <linux/time.h>
#include "helper.c"

long start_time_s;
s64 uptime_ms;

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
	// beginning a new sequence ?
	if (*pos == 0)
	{
		// yes => return a non null value to begin the sequence
		return &counter;
	}
	else
	{
		// no => it's the end of the sequence, return end to stop reading
		*pos = 0;
		return NULL;
	}
}

static void *proc_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
	printk("Hit proc_seq_next");

	char *temp = (char *)v;
	temp++;
	(*pos)++;

	return NULL;
}

static void proc_seq_stop(struct seq_file *s, void *v)
{
	printk("Hit proc_seq_stop");
}

static long get_process_elapsed_time(struct task_struct *task)
{
	// /proc/[PID]/stat
	// #14 utime - CPU time spent in user code, measured in clock ticks
	// #15 stime - CPU time spent in kernel code, measured in clock ticks
	// #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
	// #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
	// #22 starttime - Time when the process started, measured in clock ticks
	unsigned long long start_time;
	long long elapsed_sec;

	if (task == NULL)
	{
		pr_err("An error occurred in task\n");
		return -EINVAL; // Return "Invalid argument" error
	}

	// The reason for this is that the utime value in the /proc/[pid]/stat file is measured in clock ticks,
	// whereas the utime field in the task_struct is measured in nanoseconds.
	start_time = task->start_time;

	elapsed_sec = (long long)(uptime_ms * 1000000) - start_time;

	return elapsed_sec;
}

// Since version 4.14 of Linux kernel, vfs_read and vfs_write functions are no longer exported for use in modules.
// Instead, functions exclusively for kernel's file access are provided:
// # Read the file from the kernel space.
// ssize_t kernel_read(struct file *file, void *buf, size_t count, loff_t *pos);
// # Write the file from the kernel space.
// ssize_t kernel_write(struct file *file, const void *buf, size_t count, loff_t *pos);
// Also, filp_open no longer accepts user-space string, so it can be used for kernel access directly (without dance with set_fs).
// Additionally, reading file from kernel is not recommended 
static int proc_seq_show(struct seq_file *s, void *v)
{
	printk("Hit proc_seq_show");

	loff_t *spos = (loff_t *)v;
	unsigned long long utime, stime;
	unsigned long long total_time;
	char *comm;
	struct task_struct *task;

	// kernel system timer
	uptime_ms = ktime_to_ms(ktime_get_boottime());

	for_each_process(task)
	{
		// printk(KERN_INFO "Process: %s (pid: %d)\n", task->comm, task->pid);

		utime = task->utime;
		stime = task->stime;
		total_time = utime + stime;
		long elapsed_time = get_process_elapsed_time(task);

		comm = task->comm;

		// "PID\t NAME\t ELAPSED_TIME\t TOTAL_TIME\t utime\t stime\t start_time\t uptime\t\n"
		seq_printf(s,
				   "%d\t %s\t %ld\t %lld\t %lld\t %lld\t %lld\t %lld\t\n ",
				   task->pid,
				   comm,
				   elapsed_time,
				   total_time,
				   task->utime,
				   task->stime,
				   task->start_time,
				   uptime_ms);
	}

	return 0;
}

static struct seq_operations proc_seq_ops = {
	.start = proc_seq_start,
	.next = proc_seq_next,
	.stop = proc_seq_stop,
	.show = proc_seq_show};

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

#ifdef HAVE_PROC_OPS
static const struct proc_ops proc_file_fops = {
	.proc_open = procfile_open,
	.proc_write = procfile_write,
	.proc_read = seq_read,
	.proc_lseek = seq_lseek,
	.proc_release = seq_release};
#else
static const struct file_operations proc_file_fops = {
	.owner = THIS_MODULE,
	.open = procfile_open,
	.write = procfile_write,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release};
#endif

static int __init init_kernel_module(void)
{
	printk(KERN_INFO "Process logger module loaded\n");

	ktime_t start_time = ktime_get();
	s64 start_time_ns = ktime_to_ns(start_time);
	start_time_s = start_time_ns / 1000000000;

	struct proc_dir_entry *log_file;

	// printk(KERN_INFO "There are %d running processes.\n", proc_count());

	log_file = proc_create("log_file", 0, NULL, &proc_file_fops);

	return 0;
}

static void __exit exit_kernel_module(void)
{
	remove_proc_entry("log_file", NULL);

	printk(KERN_INFO "Process logger module unloaded\n");
}

module_init(init_kernel_module);
module_exit(exit_kernel_module);