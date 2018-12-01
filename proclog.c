//Logger that creates a proc file
//idea from tldp.org/LDP/lkmpg/2.6/html/index.html
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/uaccess.h>

#ifndef __KERNEL__
#define __KERNEL__
#endif

//size of buffer ~32Kb
#define PROCFS_MAX_SIZE	32768

//buffer to hold information from log
static char procfs_buffer[PROCFS_MAX_SIZE];

//size of buffer
static unsigned long procfs_buffer_size = 0;

//pointer for buffer location in read
static char *buff_ptr;

//struct to hold info about proc file
struct proc_dir_entry *log_file;

static void *proc_seq_start(struct seq_file *s, loff_t *pos){
	
	printk("Start of sequence read!\n");

	//if pos is greater than or equal to buffer size then leave sequence read 
	if((*pos) >= procfs_buffer_size){
		printk("End sequence read\n");
		return NULL;
	}
	
	buff_ptr = procfs_buffer + ((*pos) * sizeof(char));

	printk("Place in buffer is: %Ld\n", (*pos));

	return buff_ptr;
}

static void *proc_seq_next(struct seq_file *s, void *v, loff_t *pos){
	printk("Sequence Next!");
	char *temp = (char*) v;
	while((*temp) != '\n'){
		(*pos)++;
		if((*pos) >= procfs_buffer_size){
			return NULL;
		}
		temp++;
	}
	temp++;
	return temp;
}

static void proc_seq_stop(struct seq_file *s, void *v){
	printk("Sequence stop!");
	buff_ptr = NULL;
}

static int proc_seq_show(struct seq_file *s, void *v){
	printk("Showing value");
	char *temp = (char*)v;
	do{
		seq_putc(s,*temp);
		temp++;
	}while(*temp != '\n');
	seq_putc(s,'\n');
	return 0;
}


static struct seq_operations proc_seq_ops = {
	.start = proc_seq_start,
	.next = proc_seq_next,
	.stop = proc_seq_stop,
	.show = proc_seq_show
};

static int procfile_open(struct inode *inode, struct file *file){
	return seq_open(file, &proc_seq_ops);
}

//function to write to proc file
static ssize_t procfile_write(struct file *file, const char *buffer, size_t count, loff_t * off){
	//set buffer size
	procfs_buffer_size += count;
	if(procfs_buffer_size > PROCFS_MAX_SIZE){
		procfs_buffer_size = PROCFS_MAX_SIZE;
		printk("Proc file buffer overflow");
	}
	else{
		printk("Buffer size updated to: %lu", procfs_buffer_size);
	}

	//write data to buffer
	if(copy_from_user(procfs_buffer+(procfs_buffer_size - count), buffer, count)){
		return -EFAULT;
	}

	return count;
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
	log_file = proc_create("timing_log", 0, NULL, &log_file_fops);
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

