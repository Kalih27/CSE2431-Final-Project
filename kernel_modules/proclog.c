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

unsigned long virtu2phys(struct mm_struct *mm, unsigned long vpage)
{
	unsigned long physical_page_addr;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	struct page *page;
	pgd = pgd_offset(mm, vpage);
	if (pgd_none(*pgd) || pgd_bad(*pgd))
		return 0;
	p4d = p4d_offset(pgd, vpage);
	if (p4d_none(*p4d) || p4d_bad(*p4d))
		return 0;
	pud = pud_offset(p4d, vpage);
	if (pud_none(*pud) || pud_bad(*pud))
		return 0;
	pmd = pmd_offset(pud, vpage);
	if (pmd_none(*pmd) || pmd_bad(*pmd))
		return 0;
	if (!(pte = pte_offset_map(pmd, vpage)))
		return 0;
	if (!(page = pte_page(*pte)))
		return 0;
	physical_page_addr = page_to_phys(page);
	pte_unmap(pte);
	// handle unmapped pages
	if (physical_page_addr == 70368744173568)
		return 0;
	return physical_page_addr;
}

int proc_count(void)
{
	int i = 0, counter = 0, contigCounter = 0, nonContigCounter = 0;
	struct task_struct *task;
	struct vm_area_struct *vma = 0;
	unsigned long vpage;
	printk("PROCESS REPORT:\n");
	printk("proc_id,proc_name,contig_pages,noncontig_pages,total_pages\n");
	for_each_process(task)
	{
		int totalPages = 0;
		int contig = 0, nonContig = 0;
		if (task->pid > 650)
		{
			if (task->mm && task->mm->mmap)
			{
				unsigned long previous_page_addr;
				for (vma = task->mm->mmap; vma; vma = vma->vm_next)
				{
					for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE)
					{
						unsigned long physical_page_addr = virtu2phys(task->mm, vpage);
						if (physical_page_addr != 0)
						{
							if (physical_page_addr == previous_page_addr + PAGE_SIZE)
							{
								contig++;
							}
							else
							{
								nonContig++;
							}
							previous_page_addr = physical_page_addr;
							totalPages++;
						}
					}
				}
			}
			counter += totalPages;
			printk("%d,%s,%d,%d,%d\n", task->pid, task->comm, contig, nonContig, totalPages);
			i++;
		}
		contigCounter += contig;
		nonContigCounter += nonContig;
	}
	printk("TOTALS,%d,%d,%d\n", contigCounter, nonContigCounter, counter);
	return i;
}

static void *proc_seq_start(struct seq_file *s, loff_t *pos)
{
	// printk("Hit proc_seq_start");

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
	// printk("Hit proc_seq_next");

	char *temp = (char *)v;
	temp++;
	// printk("Temp increased.");
	(*pos)++;
	// printk("Position increased.");
	// printk("Position is %Ld\n", (*pos));
	return NULL;
}

static void proc_seq_stop(struct seq_file *s, void *v)
{
	// printk("Hit proc_seq_stop");
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
	// unsigned long long utime, stime, cutime, cstime;
	unsigned long long start_time;
	// unsigned long long utime_sec, stime_sec, start_time_sec;
	// unsigned long long utime_msec, stime_msec, start_time_msec;
	// unsigned long long total_time;
	// long long cpu_usage = 0;
	// long long elapsed_nsec;
	// long long usage_nsec;
	long long elapsed_sec;
	// long long usage_sec;
	// int clk_tck = 100; // constants
	// int number_of_cpu = 2;

	if (task == NULL)
	{
		pr_err("An error occurred in task\n");
		return -EINVAL; // Return "Invalid argument" error
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
	// printk("Hit proc_seq_show");

	loff_t *spos = (loff_t *)v;

	unsigned long long utime, stime;
	// unsigned long long cutime, cstime, start_time;
	unsigned long long total_time;
	struct task_struct *task;

	seq_printf(s,
			   "PID\t NAME\t ELAPSED_TIME\t TOTAL_TIME\t utime\t stime\t start_time\t uptime\t\n");
	for_each_process(task)
	{
		// printk(KERN_INFO "Process: %s (pid: %d)\n", task->comm, task->pid);

		utime = task->utime;
		stime = task->stime;

		total_time = utime + stime;
		/* Get CPU usage for the process */
		long elapsed_time = get_process_elapsed_time(task);

		seq_printf(s,
				   "%d\t %s\t %ld\t %lld\t %lld\t %lld\t %lld\t %lld\t\n ",
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

// static int procfile_single_open(struct inode *inode, struct file *file)
// {
// 	printk("Hit procfile_single_open");
// 	return single_open(file, proc_seq_show, NULL);
// }

static int procfile_open(struct inode *inode, struct file *file)
{
	// printk("Hit procfile_open");
	return seq_open(file, &proc_seq_ops);
}

static ssize_t procfile_write(struct file *file, const char *buffer, size_t count, loff_t *off)
{
	// printk("Hit procfile_write");
	return 1;
}

// static int procfile_show(struct seq_file *m, void *v)
// {
// 	printk("Hit procfile_show");
// 	return 0;
// }

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

	printk(KERN_INFO "There are %d running processes.\n", proc_count());

	// adapted from stackoverflow.com/questions/8516021/proc-create-example-for-kernel-module
	// fixed the version issue from https://stackoverflow.com/questions/64931555/how-to-fix-error-passing-argument-4-of-proc-create-from-incompatible-pointer
	log_file = proc_create("log_file", 0, NULL, &proc_file_fops);

	return 0;
}

static void export_virtual_file_into_actual_file(void)
{
	// Allocate a buffer to read data from the virtual file
	char *buffer = (char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (!buffer)
	{
		pr_err("Failed to allocate memory for buffer\n");
	}

	// // Open the virtual file
	// virtual_file = filp_open(PROC_FILE_PATH, O_RDONLY, 0);
	// if (IS_ERR(virtual_file))
	// {
	// 	pr_err("Failed to open virtual file\n");
	// }

	// Create the actual file on disk
	actual_file = filp_open(ACTUAL_FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (IS_ERR(actual_file))
	{
		pr_err("Failed to create actual file\n");
	}

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

static void __exit exit_kernel_module(void)
{
	export_virtual_file_into_actual_file();
	remove_proc_entry("log_file", NULL);
	printk(KERN_INFO "Process logger module unloaded\n");
}

module_init(init_kernel_module);
module_exit(exit_kernel_module);