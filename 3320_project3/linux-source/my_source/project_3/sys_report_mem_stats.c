// Nicholas Moreland 1001886051 - CSE 3320 Project 3 Part 1

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(report_mem_stats, pid_t, pid)
{
    struct task_struct *task;
    struct vm_area_struct *vma;
    unsigned long size = 0;
    char placehold[32] = "[ Not Available ]";
    char perms[4] = "----";
    char *name;

    task = find_task_by_vpid(pid);
    if (!task)
    {
        printk(KERN_ERR "Invalid PID\n");
        return -EINVAL;
    }

    printk(KERN_INFO "Virtual memory stats for PID %d:\n", pid);

    for (vma = task->mm->mmap; vma; vma = vma->vm_next)
    {
        if (vma->vm_file)
            name = vma->vm_file->f_path.dentry->d_iname;
        else
            name = placehold;

        perms[0] = (vma->vm_flags & VM_READ) ? 'r' : '-';
        perms[1] = (vma->vm_flags & VM_WRITE) ? 'w' : '-';
        perms[2] = (vma->vm_flags & VM_EXEC) ? 'x' : '-';
        perms[3] = (vma->vm_flags & VM_SHARED) ? 's' : 'p';
        
        size += vma->vm_end - vma->vm_start;
        printk(KERN_INFO "  Virtual memory area: 0x%lx-0x%lx Permissions: %c%c%c%c File: %s\n", vma->vm_start, vma->vm_end, perms[0], perms[1], perms[2], perms[3], name);
    }

    printk(KERN_INFO "Virtual memory size: %lu\n", size);

    return 0;
}
