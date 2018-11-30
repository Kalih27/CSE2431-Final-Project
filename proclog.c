//Logger that creates a proc file
//idea from tldp.org/LDP/lkmpg/2.6/html/index.html
#include <linux/module.h>
#include <linux/kernal.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>

#ifndef __KERNEL__
#define __KERNEL__
#endif

//size of buffer ~32Kb
#define PROCFS_MAX_SIZE	32768

//buffer to hold information from log
static char procfs_buffer[PROCFS_MAX_SIZE];

//size of buffer
static unsigned long procfs_buffer_size = 0;

//struct to hold info about proc file
struct proc_dir_entry *log_file;

static void *proc_seq_start(struct seq_file *s, loff_t *pos){
	static unsigned long counter = 0;
	
	printk("Start of sequence read!");

	if(*pos ==0){
		return &counter;
	}
	else{
		*pos = 0;
		return NULL;
	}
}

static void *proc_seq_next(struct seq_file *s, void *v, loff_t *pos){
	printk("Sequence Next!");
	unsigned long *tmp_v = (unsigned long *)v;
	(*tmp_v)++;
	(*pos)++;
	return NULL;
}

static void proc_seq_stop(struct seq_file *s, void *v){
	//nothing
}

static int proc_seq_show(struct seq_file *s, void *v){
	loff_t *spos = (loff_t *) v;

	seq_printf(s, "%Ld\n", *spos);
	return 0;
}


static struct seq_operations proc_seq_ops = {
	.start = proc_seq_start,
	.next = proc_seq_next,
	.stop = proc_seq_stop
	.show = proc_seq_show
};

static int procfile_open(struct inode *inode, struct file *file){
	return seq_open(file, &proc_seq_ops);
}

//function to write to proc file
int procfile_write(struct file *file, const char *buffer, unsigned long count, void *data){
	//set buffer size
	if(count > PROCFS_MAX_SIZE){
		procfs_buffer_size = PROCFS_MAX_SIZE;
		printk("Proc file buffer overflow");
	}
	else{
		procfs_buffer_size = count;
		printk("Buffer size updated to: %ud", count);
	}

	//write data to buffer
	if(copy_From_user(procfs_buffer, buffer, profs_buffer_size)){
		return -EFAULT;
	}

	return procfs_buffer_size;
}



//struct that holds what functions run for different aspects of log file
static const struct file_operations log_file_fops = {
 .owner = THIS_MODULE,
 .open = procfile_open,
 .read = seq_read,
 .write = procfile_write,
 .llseek = seq_lseek,
 .release = single_release,
};

int __init init_MyKernelModule(void){
	//adapted from stackoverflow.com/questions/8516021/proc-create-example-for-kernel-module
	proc_file_entry = proc_create("timing_log", 0, NULL, &log_file_fops);
	if(log_file == NULL){
		return -ENOMEM;
	}
	return 0;
}

void __exit exit_MyKernelModule(void){

	remove_proc_entry("timing_log",NULL);
	return;
}

module_init(init_MyKernelModule);
module_exit(exit_MyKernelModule);

MODULE_LISCENSE("GPL v2");

