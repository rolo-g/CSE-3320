// Rolando Rosales 1010850424 - CSE 3320 Project 2 Assignment 2

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define BUFFER_SIZE 5

char buffer[BUFFER_SIZE];
uint16_t in = 0;
uint16_t out = 0;
uint16_t count = 0;
uint16_t done = 0;
uint16_t active = 0;
uint16_t num_threads = 0;
pthread_mutex_t adding_mutex;
pthread_mutex_t active_mutex;
pthread_cond_t full_cv;
pthread_cond_t empty_cv;

void *producer(void *arg)
{
    FILE *fp;
    char c;

    fp = fopen("message.txt", "r");
    if (fp == NULL)
    {
        printf("Failed to open file");
        exit(1);
    }
    
    // blocking function that waits until all consumers are ready
    while (active < num_threads)
    {
        pthread_mutex_unlock(&active_mutex);
        pthread_mutex_lock(&active_mutex);
    }
    
    #ifdef DEBUG
        printf("Producer has arrived\n");
    #endif

    while ((c = fgetc(fp)) != EOF && c != '\n')
    {
        pthread_mutex_lock(&adding_mutex);

        while (count == BUFFER_SIZE)
            pthread_cond_wait(&empty_cv, &adding_mutex);

        buffer[in] = c;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        
        pthread_mutex_unlock(&adding_mutex);
        pthread_cond_signal(&full_cv);
    }
    
    done = 1;
    
    // blocking function that waits until all Consumer threads have finished
    while(active > 0)
    {
    	pthread_mutex_unlock(&active_mutex);
        pthread_cond_broadcast(&full_cv);
        pthread_mutex_lock(&active_mutex);
    }
    
    #ifdef DEBUG
        printf("the ship has sailed***************************************\n");
    #endif
    
    fclose(fp);
    
    pthread_exit(NULL);
}

void *consumer(void *arg)
{
    uint16_t tid = *(uint16_t *) arg;
    char c;
    
    #ifdef DEBUG
        printf("Consumer %d exists\n", tid);
    #endif
    
	pthread_mutex_lock(&active_mutex);
    active++;
    pthread_mutex_unlock(&active_mutex);

    while (!done || count > 0)
    {
        pthread_mutex_lock(&adding_mutex);
        
        if (count)
        {
            c = buffer[out];
            buffer[out] = 0;
            out = (out + 1) % BUFFER_SIZE;
            count--;

            printf("Consumer %d: %c\n", tid, c);
        }
        else
            pthread_cond_wait(&full_cv, &adding_mutex);

        #ifdef DEBUG
            printf(" consumer%d running, done%d, count%d\n", tid, done, count);  
        #endif

        pthread_mutex_unlock(&adding_mutex);
        pthread_cond_signal(&empty_cv);
    }
    
    #ifdef DEBUG
        printf("  consumer%d: has sailed*********************adios\n", tid, c);
    #endif
    
	pthread_mutex_lock(&active_mutex);
    active--;
	pthread_mutex_unlock(&active_mutex);
	
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        printf("ERROR: No command line arguments (number of consumers)\n");
        return -1;
    }
    
    num_threads = atoi(argv[1]);
    
    if (num_threads <= 0)
    {
        printf("ERROR: number of threads lower than or equal to zero\n");
        return -1;
    }

    pthread_t producer_thread, consumer_threads[num_threads];
    uint16_t pid[num_threads];
    uint16_t i;
    uint16_t rc;
    void *status;

    pthread_mutex_init(&adding_mutex, NULL);
    pthread_mutex_init(&active_mutex, NULL);
    pthread_cond_init(&full_cv, NULL);
    pthread_cond_init(&empty_cv, NULL);
    
    for (i = 0; i < num_threads; i++)
    {
        pid[i] = i;
        rc = pthread_create(&consumer_threads[i], NULL, consumer, &pid[i]);
		if (rc)
		{
			printf("ERROR; return code from pthread_create() is %d\n", rc);
			exit(-1);
		}
    }
    pthread_create(&producer_thread, NULL, producer, NULL);
    
    for (i = 0; i < num_threads; i++)
    {
        rc = pthread_join(consumer_threads[i], &status);
        if (rc)
        {
             printf("ERROR; return code from pthread_join() is %d\n", rc);
             exit(-1);
        }
    }
    pthread_join(producer_thread, NULL);
    
    pthread_mutex_destroy(&adding_mutex);
    pthread_mutex_destroy(&active_mutex);
    pthread_cond_destroy(&full_cv);
    pthread_cond_destroy(&empty_cv);
    
    pthread_exit(NULL);
}
