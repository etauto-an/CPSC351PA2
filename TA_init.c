// This is the initial starter code without separation

#include <pthread.h>    //Create POSIX threads.
#include <semaphore.h>  //To create semaphores
#include <stdio.h>      //Input Output
#include <stdlib.h>
#include <time.h>    //Wait for a random time.
#include <unistd.h>  //Thread calls sleep for specified number of seconds.

pthread_t *Students;                 // N threads running as Students.
pthread_t TA;                        // Separate Thread for TA.
pthread_mutex_t office_hours_mutex;  // Mutex to protect office_hours_over

int ChairsCount = 0;
int CurrentIndex = 0;
int office_hours_over =
    0;  // 0 means office hours are ongoing, 1 means office hours are over

// Declaration of Semaphores and Mutex Lock.
// Semaphores used:
// A binary semaphore to signal and wait TA's sleep.
sem_t ta_status;
// An array of 3 semaphores to signal and wait chair to wait for the TA.
sem_t chair[3];
// A semaphore to signal and wait for TA's next student.
sem_t next_student;

// Mutex Lock used:
// To lock and unlock variable ChairsCount to increment and decrement its value.
pthread_mutex_t mutex;
// hint: use sem_t and pthread_mutex_t

// Declared Functions
void *TA_Activity(void *arg);
void *Student_Activity(void *threadID);

int main(int argc, char *argv[]) {
  int number_of_students =
      0;  // a variable taken from the user to create student threads.	Default
          // is 5 student threads.
  // int id = 0; <--- purpose?
  srand(time(NULL));

  // Initializing Mutex Lock and Semaphores.

  // Initialize as 0 - TA is initially asleep.
  sem_init(&ta_status, 0, 0);

  // Initialize semaphore array elements as 1 - Chair is available.
  for (int i = 0; i < 3; i++) {
    sem_init(&chair[i], 0, 1);
  }

  // Initialize as 0 - No students waiting
  sem_init(&next_student, 0, 0);

  // Initialize mutex locks
  pthread_mutex_init(&mutex, NULL);
  pthread_mutex_init(&office_hours_mutex, NULL);

  // hint: use sem_init() and pthread_mutex_init()

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
  // hint: use pthread_create

  // TA thread
  pthread_create(&TA, NULL, TA_Activity, NULL);

  // Student threads
  for (int i = 0; i < number_of_students; i++) {
    pthread_create(&Students[i], NULL, Student_Activity, (void *)(__intptr_t)i);
  }

  // Waiting for TA thread and N Student threads.
  // hint: use pthread_join
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

void *TA_Activity(void *arg) {
  while (1) {
    // TA is currently sleeping.
    sem_wait(&ta_status);

    // lock
    pthread_mutex_lock(&mutex);

    // if chairs are empty, break the loop.
    if (ChairsCount == 0) {
      pthread_mutex_unlock(&mutex);
      continue;
    }

    // TA gets next student on chair.
    ChairsCount--;

    // unlock
    pthread_mutex_unlock(&mutex);

    // TA is currently helping the student

    // Signal the next student to come in
    sem_post(&chair[CurrentIndex]);

    // Update the index to point to the next student waiting (using circular
    // queue)
    CurrentIndex = (CurrentIndex + 1) % 3;

    // Simulate TA helping the student
    sleep(rand() % 5 + 1);

    // hint: use sem_wait(); sem_post(); pthread_mutex_lock();
    // pthread_mutex_unlock()

    // TA is ready for the next student
    sem_post(&next_student);
  }
  pthread_exit(NULL);
}

void *Student_Activity(void *threadID) {
  /*TODO

      //Student  needs help from the TA
      //Student tried to sit on a chair.
      //wake up the TA.
  // lock

      // unlock
  //Student leaves his/her chair.
      //Student  is getting help from the TA
      //Student waits to go next.
      //Student left TA room

  //If student didn't find any chair to sit on.
  //Student will return at another time

   //hint: use sem_wait(); sem_post(); pthread_mutex_lock();
  pthread_mutex_unlock()

      */
  pthread_exit(NULL);
}