#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define SIZE 10

int buffer[SIZE];
int in = 0;
int out = 0;

sem_t empty;
sem_t full;
sem_t mutex;

// Producer
void *producer(void *arg)
{
    int item;

    for (int i = 1; i <= 10; i++)
    {
        item = i;

        // Wait for an empty slot
        sem_wait(&empty);

        // Enter critical section
        sem_wait(&mutex);

        buffer[in] = item;
        printf("Producer produces item %d\n", item);

        in = (in + 1) % SIZE;

        // Leave critical section
        sem_post(&mutex);

        // Increase full slots
        sem_post(&full);

        sleep(1);
    }

    return NULL;
}

// Consumer
void *consumer(void *arg)
{
    int item;

    for (int i = 1; i <= 10; i++)
    {
        // Wait for a full slot
        sem_wait(&full);

        // Enter critical section
        sem_wait(&mutex);

        item = buffer[out];
        printf("Consumer consumes item %d\n", item);

        out = (out + 1) % SIZE;

        // Leave critical section
        sem_post(&mutex);

        // Increase empty slots
        sem_post(&empty);

        sleep(1);
    }

    return NULL;
}

int main()
{
    pthread_t producerThread, consumerThread;

    
    sem_init(&empty, 0, SIZE);
    sem_init(&full, 0, 0);
    sem_init(&mutex, 0, 1);

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&mutex);

    return 0;
}
