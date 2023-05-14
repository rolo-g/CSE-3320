// OpenAI ChatGPT - CSE 3320 Project 2 Assignment 2

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 5
#define MESSAGE_FILE "message.txt"

char buffer[BUFFER_SIZE];
int in = 0;
int out = 0;
int count = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;

void *producer(void *arg) {
    FILE *fp;
    char ch;

    fp = fopen(MESSAGE_FILE, "r");
    if (fp == NULL) {
        perror("Failed to open file");
        exit(1);
    }

    while ((ch = fgetc(fp)) != EOF) {
        pthread_mutex_lock(&mutex);

        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&empty, &mutex);
        }

        buffer[in] = ch;
        in = (in + 1) % BUFFER_SIZE;
        count++;

        pthread_cond_signal(&full);
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_lock(&mutex);
    while (count > 0) {
        pthread_cond_wait(&empty, &mutex);
    }
    pthread_mutex_unlock(&mutex);

    fclose(fp);

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int id = *(int *) arg;
    char ch;

    while (1) {
        pthread_mutex_lock(&mutex);

        while (count == 0) {
            pthread_cond_wait(&full, &mutex);
        }

        ch = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;

        printf("Consumer %d: %c\n", id, ch);

        pthread_cond_signal(&empty);
        pthread_mutex_unlock(&mutex);
    }
}

int main() {
    pthread_t producer_thread, consumer_threads[10];
    int consumer_ids[10];
    int i;

    pthread_create(&producer_thread, NULL, producer, NULL);

    for (i = 0; i < 10; i++) {
        consumer_ids[i] = i;
        pthread_create(&consumer_threads[i], NULL, consumer, &consumer_ids[i]);
    }

    pthread_join(producer_thread, NULL);
    for (i = 0; i < 10; i++) {
        pthread_join(consumer_threads[i], NULL);
    }

    return 0;
}
