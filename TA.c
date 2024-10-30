// File: main.c
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#include "Student.h"
#include "TA_Activity.h"

// Shared variables
pthread_t *Students;                 // N threads running as Students.
pthread_t TA;                        // Separate thread for TA.
pthread_mutex_t mutex;               // Mutex for shared resources.
pthread_mutex_t office_hours_mutex;  // Mutex to protect office_hours_over
int ChairsCount = 0;                 // Number of waiting students.
int CurrentIndex = 0;                // Index of the next waiting student.
int office_hours_over =
    0;  // 0 means office hours are ongoing, 1 means office hours are over

// Semaphores
sem_t ta_status;     // Semaphore for TA sleep/wake.
sem_t chair[3];      // Semaphore array for chairs.
sem_t next_student;  // Semaphore for next student.

int main(int argc, char *argv[]) {
  int number_of_students = 0;  // a variable taken from the user to create
                               // student threads. Default is 5 student threads.
  srand(time(NULL));

  // Initializing Mutex Lock and Semaphores.
  sem_init(&ta_status, 0, 0);  // Initialize as 0 - TA is initially asleep.
  for (int i = 0; i < 3; i++) {
    sem_init(
        &chair[i], 0,
        1);  // Initialize semaphore array elements as 1 - Chair is available.
  }
  sem_init(&next_student, 0, 0);  // Initialize as 0 - No students waiting

  // Initialize mutex locks
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&office_hours_mutex, NULL);

  if (argc < 2) {
    printf("Number of Students not specified. Using default (5) students.\n");
    number_of_students = 5;
  } else {
    printf("Number of Students specified. Creating %d threads.\n",
           number_of_students);
    number_of_students = atoi(argv[1]);
  }

  // Allocate memory for Students
  Students = (pthread_t *)malloc(sizeof(pthread_t) * number_of_students);

  // Creating one TA thread and N Student threads.
  pthread_create(&TA, NULL, TA_Activity, NULL);
  for (int i = 0; i < number_of_students; i++) {
    pthread_create(&Students[i], NULL, Student_Activity, (void *)(__intptr_t)i);
  }

  // Waiting for TA thread and N Student threads.
  pthread_join(TA, NULL);
  for (int i = 0; i < number_of_students; i++) {
    pthread_join(Students[i], NULL);
  }

  // Destroy mutex locks
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&office_hours_mutex);

  // Destroy semaphores
  sem_destroy(&ta_status);
  sem_destroy(&next_student);
  for (int i = 0; i < 3; i++) {
    sem_destroy(&chair[i]);
  }

  // Free allocated memory
  free(Students);
  return 0;
}
