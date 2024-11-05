#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Student.h"
#include "TA_Activity.h"

// Shared variables
pthread_t *students;                 // N threads running as Students
pthread_t ta;                        // Separate thread for TA
pthread_mutex_t mutex;               // Mutex for shared resources
pthread_mutex_t office_hours_mutex;  // Mutex to protect office_hours_over
//pthread_cond_t office_hours_cond =
//    PTHREAD_COND_INITIALIZER;  // Condition variable to signal end of office
                               // hours
int StudentsWaiting = 0;           // Number of waiting students
int CurrentIndex = 0;          // Index of the next waiting student
int office_hours_over =
    0;  // 0 means office hours are ongoing, 1 means office hours are over

// Semaphores
sem_t ta_status;     // Semaphore for TA sleep/wake
sem_t chair[3];      // Semaphore array for chairs
sem_t next_student;  // Semaphore for next student
sem_t ta_chair;

int main(int argc, char *argv[]) {
  int number_of_students = 0;  // A variable taken from the user to create
                               // student threads. Default is 5 student threads.
  srand(time(NULL));

  // Initializing Mutex Lock and Semaphores
  sem_init(&ta_status, 0, 0);  // Initialize as 0 - TA is initially asleep
  for (int i = 0; i < 3; i++) {
    sem_init(
        &chair[i], 0,
        1);  // Initialize semaphore array elements as 1 - Chair is available
  }
  sem_init(&next_student, 0, 0);  // Initialize as 0 - No students waiting
  sem_init(&ta_chair, 0, 0);

  // Initialize mutex locks
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&office_hours_mutex, NULL);

  if (argc < 2) {
    printf("Number of students not specified. Using default (5) students.\n");
    number_of_students = 5;
  } else {
    number_of_students = atoi(argv[1]);
    printf("Number of students specified. Creating %d threads.\n",
           number_of_students);
  }

  // Allocate memory for Students
  students = (pthread_t *)malloc(sizeof(pthread_t) * number_of_students);

  // Creating one TA thread and N Student threads
  pthread_create(&ta, NULL, TA_Activity, NULL);
  for (int i = 0; i < number_of_students; i++) {
    pthread_create(&students[i], NULL, Student_Activity, (void *)(__intptr_t)i);
  }

  /**
  // Simulate end of office hours after a while (for testing purposes)
  // sleep(20);  // Simulate office hours duration
  sleep(5);  // TEST
  pthread_mutex_lock(&office_hours_mutex);
  office_hours_over = 1;  // Set office hours to over
  pthread_mutex_unlock(&office_hours_mutex);
  pthread_cond_broadcast(&office_hours_cond);  // Wake up all threads waiting
                                               // for office hours to end
  sem_post(&ta_status);                        // Wake up the TA if sleeping
  **/

  // Waiting for TA thread and N Student threads
  pthread_join(ta, NULL);
  for (int i = 0; i < number_of_students; i++) {
    pthread_join(students[i], NULL);
  }

  // Destroy mutex locks
  pthread_mutex_destroy(&mutex);
  pthread_mutex_destroy(&office_hours_mutex);

  // Destroy semaphores
  sem_destroy(&ta_status);
  sem_destroy(&ta_chair);

  sem_destroy(&next_student);

  for (int i = 0; i < 3; i++) {
    sem_destroy(&chair[i]);
  }

  // Free allocated memory
  free(students);

  printf("Program END\n");
  return 0;
}
