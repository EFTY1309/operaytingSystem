#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N 5 // Number of philosophers

sem_t chopsticks[N];

void *philosopher(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % N;

    // Thinking
    sleep(1);

    // Start eating
    sem_wait(&chopsticks[left]);
    sem_wait(&chopsticks[right]);

    printf("Philosopher %d is eating\n", id);
    sleep(2); // Simulating eating

    // Done eating
    sem_post(&chopsticks[left]);
    sem_post(&chopsticks[right]);

    printf("Philosopher %d is thinking\n", id);

    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_t philosophers[N];
    int ids[N];

    for (int i = 0; i < N; i++) {
        sem_init(&chopsticks[i], 0, 1);
    }

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(philosophers[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        sem_destroy(&chopsticks[i]);
    }

    return 0;
}

