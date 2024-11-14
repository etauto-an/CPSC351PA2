#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Define the maximum number of students and the number of chairs in the waiting room
#define MAX_STUDENTS 5
#define NUM_CHAIRS 3

// Semaphores for the TA and students
sem_t ta_semaphore;      // Signals the TA that a student needs help
sem_t student_semaphore; // Allows a student to proceed to the TA's office

// Mutex to protect shared resources
pthread_mutex_t mutex;

// Queue of waiting students (represented by their IDs)
int waiting_students[NUM_CHAIRS];
int waiting_count = 0; // Number of students currently waiting

// TA thread function
void* TA_Activity() {
    while (1) {
        // The TA is sleeping until a student signals for help
        printf("TA is sleeping...\n");
        sem_wait(&ta_semaphore);  // Wait until a student needs help

        // Acquire mutex to access shared resources
        pthread_mutex_lock(&mutex);
        
        if (waiting_count > 0) {
            // There's a student waiting; the TA will help them

            int student_id = waiting_students[0]; // Get the first waiting student
            // Shift the queue to remove the student being helped
            for (int i = 0; i < waiting_count - 1; i++) {
                waiting_students[i] = waiting_students[i + 1];
            }
            waiting_count--;

            printf("TA is helping student %d\n", student_id);
            pthread_mutex_unlock(&mutex);  // Release mutex so other students can queue
            
            // Simulate helping the student
            sleep(rand() % 3 + 1);

            // Signal the student that they are done
            printf("TA has finished helping student %d\n", student_id);
            sem_post(&student_semaphore);
        } else {
            // No students waiting (shouldn't reach here under normal conditions)
            pthread_mutex_unlock(&mutex);
        }
    }
    pthread_exit(NULL);
}

// Student thread function
void* student_activity(void* arg) {
    int student_id = *(int*)arg;
    free(arg);  // Free the dynamically allocated student ID

    // Arrive at the TA's office at a random time
    sleep(rand() % 5 + 1);

    // Attempt to acquire a seat in the waiting room
    pthread_mutex_lock(&mutex);
    if (waiting_count < NUM_CHAIRS) {
        // Seat available, student takes a seat
        waiting_students[waiting_count] = student_id;
        waiting_count++;

        printf("Student %d is waiting in the waiting room.\n", student_id);
        pthread_mutex_unlock(&mutex);  // Release mutex for other students

        // Signal the TA that a student needs help
        sem_post(&ta_semaphore);

        // Wait until the TA calls the student for help
        sem_wait(&student_semaphore);
        printf("Student %d has finished getting help.\n", student_id);
    } else {
        // No seats available, student leaves
        printf("Student %d found no available chairs and is leaving.\n", student_id);
        pthread_mutex_unlock(&mutex);  // Release mutex
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));  // Seed for random number generation

    // Initialize semaphores and mutex
    sem_init(&ta_semaphore, 0, 0);        // TA starts sleeping
    sem_init(&student_semaphore, 0, 0);   // No students waiting initially
    pthread_mutex_init(&mutex, NULL);

    // Create the TA thread
    pthread_t ta_thread;
    pthread_create(&ta_thread, NULL, TA_Activity, NULL);

    // Create student threads
    pthread_t student_threads[MAX_STUDENTS];
    for (int i = 0; i < MAX_STUDENTS; i++) {
        int* student_id = malloc(sizeof(int));
        *student_id = i;
        pthread_create(&student_threads[i], NULL, student_activity, student_id);
    }

    // Wait for all student threads to finish
    for (int i = 0; i < MAX_STUDENTS; i++) {
        pthread_join(student_threads[i], NULL);
    }

    // Cleanup (TA thread would normally run indefinitely in a real application)
    pthread_cancel(ta_thread);
    pthread_join(ta_thread, NULL);

    sem_destroy(&ta_semaphore);
    sem_destroy(&student_semaphore);
    pthread_mutex_destroy(&mutex);

    return 0;
}
