
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;
sem_t wrt;

int ReadCount = 0;
int data = 0;


void *reader(void *arg)
{
    int id = *(int *)arg;

    sem_wait(&mutex);

    ReadCount++;

    if (ReadCount == 1)
        sem_wait(&wrt);

    sem_post(&mutex);

    // Critical section
    printf("Reader %d is reading data: %d\n", id, data);
    sleep(1);

    sem_wait(&mutex);

    ReadCount--;

    if (ReadCount == 0)
        sem_post(&wrt);

    sem_post(&mutex);

    return NULL;
}

void *writer(void *arg)
{
    int id = *(int *)arg;

    sem_wait(&wrt);

    // Critical section
    data++;
    printf("Writer %d is writing data: %d\n", id, data);
    sleep(1);

    sem_post(&wrt);

    return NULL;
}

int main()
{
    pthread_t r[3], w[2];

    int readerID[3] = {1, 2, 3};
    int writerID[2] = {1, 2};

    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);

    for (int i = 0; i < 3; i++)
        pthread_create(&r[i], NULL, reader, &readerID[i]);

    for (int i = 0; i < 2; i++)
        pthread_create(&w[i], NULL, writer, &writerID[i]);

     for (int i = 0; i < 3; i++)
        pthread_join(r[i], NULL);

     for (int i = 0; i < 2; i++)
        pthread_join(w[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
