#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

const int NUM_STUDENTS = 5; // Number of students
const int NUM_SLICES = 8;   // Slices per pizza

sem_t pizza;    // Number of available pizza resources; initialized to 0
sem_t deliver;  // Semaphore for pizza delivery; initialized to 1
pthread_mutex_t mutex; // Mutex to protect accesses to num_slices
int num_slices = 0; // Total slices initially

// Function executed by student threads
void* studentFunction(void* id) {
    int student_id = *((int*)id);
    while (true) {
        sem_wait(&pizza);
        pthread_mutex_lock(&mutex);
        if (num_slices > 0) {
            num_slices--;
            cout << "Student " << student_id << " eats a slice. Slices left: " << num_slices << endl;
        } else {
            cout << "Student " << student_id << " found no pizza. Goes to sleep." << endl;
            pthread_mutex_unlock(&mutex);
            continue;
        }
        sem_post(&pizza);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
    pthread_exit(NULL);
}

// Function executed by the delivery thread
void* deliveryFunction(void*) {
    while (true) {
        sem_wait(&deliver);
        pthread_mutex_lock(&mutex);
        if (num_slices <= 0) {
            cout << "Delivery thread: Pizza is out! Ordering a new one..." << endl;
            num_slices = NUM_SLICES;
            sem_post(&pizza);
        }
        pthread_mutex_unlock(&mutex);
        sem_post(&deliver);
        sleep(2);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t students[NUM_STUDENTS];
    pthread_t deliveryThread;

    sem_init(&pizza, 0, 0) ;
    sem_init(&deliver, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    // Create student threads
    int student_ids[NUM_STUDENTS];
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, studentFunction, &student_ids[i]);
    }

    // Create delivery thread
    pthread_create(&deliveryThread, NULL, deliveryFunction, NULL);

    // Join student threads
    for (int i = 0; i < NUM_STUDENTS; ++i) {
        pthread_join(students[i], NULL);
    }

    // Join delivery thread
    pthread_join(deliveryThread, NULL);

    sem_destroy(&pizza);
    sem_destroy(&deliver);
    pthread_mutex_destroy(&mutex);

    return 0;
}

