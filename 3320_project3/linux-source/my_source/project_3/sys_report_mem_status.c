// Rolando Rosales 1001850424 - CSE 3320 Project 3 Part 2

#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pgtable.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE2(report_mem_status, pid_t, pid, unsigned long, addr)
{
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *ptep, pte;

    pgd = pgd_offset(find_task_by_vpid(pid)->mm, addr);
    if (pgd_none(*pgd) || pgd_bad(*pgd))
    {
        printk(KERN_ERR "PGD Error\n");
        return -ENXIO;
    }
    
    p4d = p4d_offset(pgd, addr);
    if (p4d_none(*p4d) || p4d_bad(*p4d))
    {
        printk(KERN_ERR "P4D Error\n");
        return -ENXIO;
    }
    
    pud = pud_offset(p4d, addr);
    if (pud_none(*pud) || pud_bad(*pud))
    {
        printk(KERN_ERR "PUD Error\n");
        return -ENXIO;
    }
    
    pmd = pmd_offset(pud, addr);
    if (pmd_none(*pmd) || pmd_bad(*pmd))
    {
        printk(KERN_ERR "PMD Error\n");
        return -ENXIO;
    }

    ptep = pte_offset_kernel(pmd, addr);
    if (!ptep)
    {
        printk(KERN_ERR "PTE Error\n");
        return -ENXIO;
    }

    pte = *ptep;
    
    printk(KERN_INFO "Status for PID %d with address 0x%lx:\n", pid, addr);
    printk(KERN_INFO "  Present:\t%s\n", pte_present(pte) != 0 ? "True" : "False");
    printk(KERN_INFO "  Referenced:\t%s\n", pte_young(pte) != 0 ? "True" : "False");
    printk(KERN_INFO "  Dirty:\t\t%s\n", pte_dirty(pte) != 0 ? "True" : "False");

    return 0;
}
