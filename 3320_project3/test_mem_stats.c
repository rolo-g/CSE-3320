// Nicholas Moreland 1001886051 - CSE 3320 Project 3 Part 1 Test Program 1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define __NR_report_mem_stats 338

int main(int argc, char *argv[])
{
    printf("Calling sys_report_mem_stats for current process:\n");
    syscall(__NR_report_mem_stats, atoi(argv[1]));
    return 0;
}
