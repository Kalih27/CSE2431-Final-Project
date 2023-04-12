/*
  A kernel module to list process by their names. Print when loading the module.
  In the terminal, compile this module with "make" and load it with "sudo insmod process.ko". 
  Check the result of printk by “dmesg”
  Unload it with "sudo rmmod process.ko".
*/

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched.h> // task_struct definition
#include <asm/unistd.h>
#include <linux/list.h>
#include <linux/init_task.h>

#ifndef __KERNEL__
#define __KERNEL__
#endif

// Print all running processes with their names
void print_task_by_name(void) {

  // The struct used for info of a process
  struct task_struct* task;

  // Go over through the list of processes
  for_each_process(task) {
    // Print to syslog
    printk("Current process: %s, PID: %d\n", task->comm, task->pid);
  }

  return;
}

// Initialization of module
int __init init_MyKernelModule(void)
{
  printk("Process List Lookup Module Init.\n");
  print_task_by_name();
  return 0;
}

// Exit of module
void __exit exit_MyKernelModule(void)
{
  printk("Process List Lookup Module Exit.\n");
  return;
}

module_init(init_MyKernelModule);
module_exit(exit_MyKernelModule);

MODULE_LICENSE("GPL v2");
MODULE_DESCRIPTION("A kernel module to list process by their names");
