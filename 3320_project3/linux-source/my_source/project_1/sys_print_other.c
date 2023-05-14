#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(print_other, pid_t, pid)
{
  struct task_struct *task;
  task = find_task_by_vpid(pid);
  
  if(task->pid == pid)
  {
    printk("print_other\n");
    printk("PID: %d\n", task->pid);
    printk("State: %ld\n", task->state);
    printk("Process Name: %s\n", task->comm);
    printk("Start Time: %lld\n", task->start_time);
    printk("Virtual Runtime: %lld\n", task->se.vruntime);
    printk("Parent processes:");

    for(task=task;task!=&init_task;task=task->parent)
      printk("%s\n",task->comm);
  }
  else
    printk("Error: Invalid PID");
    
  return 0;
}
