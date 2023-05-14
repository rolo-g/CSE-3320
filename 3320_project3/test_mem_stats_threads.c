#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <linux/types.h>
#include <limits.h>

struct vma_stats
{
    unsigned long long size;
    char perms[5];
    char filename[PATH_MAX];
};

#define __NR_report_mem_stats 338

void *thread_func(void *arg)
{
    pid_t tid = syscall(SYS_gettid);
    struct vma_stats stats;
    long ret = syscall(__NR_report_mem_stats, tid, &stats);

    if (ret < 0)
    {
        perror("Error calling report_mem_stats");
        exit(EXIT_FAILURE);
    }

    return NULL;
}

int main()
{
    int i, num_threads = 5;
    pthread_t threads[num_threads];

    for (i = 0; i < num_threads; i++)
    {
        if (pthread_create(&threads[i], NULL, thread_func, NULL) != 0)
        {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < num_threads; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            perror("Error joining thread");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
