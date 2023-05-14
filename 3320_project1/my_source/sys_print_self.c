#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE0(print_self)
{
  struct task_struct *task;
  
  printk("print_self\n");
  printk("PID: %d\n", current->pid);
  printk("State: %ld\n", current->state);
  printk("Process Name: %s\n", current->comm);
  printk("Start Time: %lld\n", current->start_time);
  printk("Virtual Runtime: %lld\n", current->se.vruntime);
  printk("Parent processes:");
  
  for(task=current;task!=&init_task;task=task->parent)
    printk("%s\n",task->comm);
    
  return 0;
}
