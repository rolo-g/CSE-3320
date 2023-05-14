// Rolando Rosales 1001850424 - CSE 3320 Project 3 Part 2 Test Program

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define __NR_report_mem_status 339

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        pid_t pid = atoi(argv[1]);
        unsigned long addr = strtoul(argv[2], NULL, 16);
        
        syscall(__NR_report_mem_status, pid, addr);
        
        printf("Use 'dmesg' to display kernel output\n");
    }
    else
        printf("ERROR: program takes command line arguments:\n"
        " $ %s [PID] [Address in Hex]\n", argv[0]);

  return 0;
}
