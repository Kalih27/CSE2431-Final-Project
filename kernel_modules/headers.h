/********* headers.c ***********/
#include <linux/version.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h> // seq_read, ...
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/jiffies.h>
#include <linux/syscalls.h>
#include <linux/unistd.h>
#include <linux/ktime.h>
#include <linux/types.h>
#include <linux/fs.h>

static int endflag;

// /proc/uptime #1 uptime of the system (seconds)
s64 uptime;

struct file *virtual_file = NULL;
struct file *actual_file = NULL;

#define ACTUAL_FILE_PATH "/tmp/actual_file"
#define PROC_FILE_PATH "/proc/log_file"

// // size of buffer ~32Kb
// #define PROCFS_MAX_SIZE 32768

// // buffer to hold information from log
// static char procfs_buffer[PROCFS_MAX_SIZE];

// // size of buffer
// static unsigned long procfs_buffer_size = 0;

// // pointer for buffer location in read
// static char *buff_ptr;