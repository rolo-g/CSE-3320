// Rolando Rosales 1010850424 - CSE 3320 Project 2 Assignment 1

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

#define MAX 1024

uint16_t total = 0;
uint16_t n1, n2;
char *s1, *s2;   
FILE *fp;
pthread_mutex_t total_mutex;
pthread_mutex_t string_mutex;

struct thread_data
{
    uint16_t tid;
    uint16_t start;   // start index of the index's partition
    uint16_t size;    // size of the partition
};

void *thread_main(void *arg)
{
	struct thread_data *thread_data;
	thread_data = (struct thread_data *)arg;
	
	uint16_t i = thread_data->start, curr = 0;
    while (i < thread_data->size + thread_data->start || curr)
    {
        pthread_mutex_lock(&string_mutex);
        if (*(s1 + i) == *(s2 + curr))
            curr++;
        else
            curr = 0;
        pthread_mutex_unlock(&string_mutex);
        if (curr == n2)
        {
		    pthread_mutex_lock(&total_mutex);
            total++;
            pthread_mutex_unlock(&total_mutex);
            curr = 0;
            #ifdef DEBUG
                printf("Substring found in Pthread %d at i = %d\n", thread_data->tid, i);
            #endif
        }
        i++;
    }
    pthread_exit(NULL);
}

void readf(FILE *fp)
{
    if ((fp = fopen("strings.txt", "r")) == NULL)
    {
        printf("ERROR: can't open strings.txt!\n");
        exit(-1);
    }
    
    s1 = (char *)malloc(sizeof(char) * MAX);
    if (s1 == NULL)
    {
        printf("ERROR: Out of memory! Could not set string to search\n");
        exit(-1);
    }
    
    s2 = (char *)malloc(sizeof(char) * MAX);
    if (s2 == NULL)
    {
        printf("ERROR: Out of memory! Could not set string to find\n");
        exit(-1);
    }

    // read s1 s2 from the file
    s1 = fgets(s1, MAX, fp);
    s2 = fgets(s2, MAX, fp);
    n1 = strlen(s1) - 1; // length of s1
    n2 = strlen(s2) - 1; // length of s2
    if (n1 >= MAX - 2)
    {
        printf("ERROR: String to search is too large, MAX is %d characters\n", MAX - 2);
        exit(-1);
    }
    if (n1 < n2)
    {
        printf("ERROR: String to search is larger than string to find\n");
        exit(-1);
    }
}

int main (int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("ERROR: No command line arguments (number of threads)\n");
        return -1;
    }
    
    int16_t num_threads = atoi(argv[1]);
    
    if (num_threads <= 0)
    {
        printf("ERROR: number of threads lower than or equal to zero\n");
        return -1;
    }
    
    readf(fp);
    
    uint16_t minLen = n1 / num_threads; // minimum partition length
    uint16_t leftover = n1 % num_threads;   // remainder to add to partitions
    
    if (minLen < n2)
    {
        printf("ERROR: string to search longer than each thread's string\n");
        return -1;
    }

    pthread_t threads[num_threads];
    int rc;
    uint16_t i, pad = 0, j = 0;
    struct thread_data thread_data_array[num_threads];
    void *status;

    #ifdef DEBUG
        printf("n1: %d, minLen: %d, leftover: %d\n", n1, minLen, leftover);
    #endif

    pthread_mutex_init(&total_mutex, NULL);
    pthread_mutex_init(&string_mutex, NULL);
    
	for (i = 0; i < num_threads; i++)
    {
	    pad = leftover > 0 ? 1 : 0;
	    thread_data_array[i].start = j;
		thread_data_array[i].size = minLen + pad;
		thread_data_array[i].tid = i;
		rc = pthread_create(&threads[i], NULL, thread_main, (void *) &thread_data_array[i]);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
		j += minLen + pad;
		leftover--;
	}
    
    for (i = 0; i < num_threads; i++)
    {
        rc = pthread_join(threads[i], &status);
        if (rc)
        {
             printf("ERROR; return code from pthread_join() is %d\n", rc);
             exit(-1);
        }
    }
    
    pthread_mutex_destroy(&total_mutex);
    pthread_mutex_destroy(&string_mutex);

 	printf("The number of substrings is: %d\n", total);

    pthread_exit(NULL);
}
