// Rolando Rosales 1001850424 - CSE 3320 Project 1 Test Program

#include <stdio.h>
#include <stdbool.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/types.h>

#define __NR_helloworld 335
#define __NR_print_self 336
#define __NR_print_other 337

int main(int argc, char *argv[])
{
  bool finished = false;
  int num = 0;
  pid_t pid = 0;
    
  while(!finished)
  {
    printf("Which syscall to test?"
           "\n0: helloworld"
           "\n1: print_self"
           "\n2: print_other\n");
    scanf("%d", &num);
    
    switch(num)
    {
      case 0:
      {
        syscall(__NR_helloworld);
        finished = true;
        break;
      }
      case 1:
      {
        syscall(__NR_print_self);
        printf("Enter 'sudo dmesg' to display print_self\n");
        finished = true;
        break;
      }
      case 2:
      {
        printf("Which PID to display?\n");
        scanf("%d", &pid);
        syscall(__NR_print_other, pid);
        printf("Enter 'sudo dmesg' to display print_other\n");
        finished = true;
        break;
      }
      default:
      {
        printf("Invalid option\n\n");
        break;
      }
    }    
  }

  return 0;
}
