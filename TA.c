#include <pthread.h>    // For POSIX threads
#include <semaphore.h>  // For semaphores
#include <stdio.h>      // For standard input/output
#include <stdlib.h>     // For standard library functions
#include <time.h>       // For random delays
#include <unistd.h>     // For sleep function

// Global variables and thread handles
pthread_t *Students;  // Array of student threads
pthread_t TA;         // Thread for the TA

// Shared variables
int students_waiting = 0;      // Number of students waiting outside
int next_waiting_index = 0;    // Next student to be helped
int available_seat_index = 0;  // Next available seat index for new arrivals
int ta_awake = 0;              // Status of TA (0 = asleep, 1 = awake)
int office_hours_over = 0;     // Office hours status (0 = ongoing, 1 = over)

// Semaphores
sem_t ta_status;  // Controls TA's sleep/wake status (0 = asleep, 1 = awake)
sem_t ta_ready;  // Ensures only one student can enter the office at a time

sem_t chair[3];  // Array for the 3 waiting chairs (0 = occupied, 1 = available)

// Mutexes
pthread_mutex_t sw_mutex;   // Mutex for accessing `students_waiting`
pthread_mutex_t oho_mutex;  // Mutex for accessing `office_hours_over`

// Function prototypes
void *TA_Activity();  // Function for TA's activities
void *Student_Activity(
    void *threadID);  // Function for each student's activities

int main(int argc, char *argv[]) {
  // Set default number of students if not provided via command line
  int number_of_students = 5;
  srand(time(NULL));  // Seed for random number generation

  // Initialize semaphores and mutexes
  sem_init(&ta_status, 0, 0);       // TA initially asleep (semaphore value 0)
  sem_init(&ta_ready, 0, 0);  // TA chair is available (semaphore value 1)


  for (int i = 0; i < 3; i++) {
    sem_init(&chair[i], 0, 1);  // All 3 chairs outside are initially available
  }
  pthread_mutex_init(&sw_mutex, NULL);   // Mutex for students waiting
  pthread_mutex_init(&oho_mutex, NULL);  // Mutex for office hours status

  // Capture user input to define the number of students
  if (argc < 2) {
    printf("Number of Students not specified. Using default (5) students.\n");
  } else {
    number_of_students = atoi(argv[1]);
    printf("Number of Students specified. Creating %d threads.\n\n",
           number_of_students);
  }

  // Allocate memory for the Student thread array
  Students = (pthread_t *)malloc(sizeof(pthread_t) * number_of_students);

  // Create the TA thread
  pthread_create(&TA, NULL, TA_Activity, NULL);

  // Create Student threads
  for (int i = 0; i < number_of_students; i++) {
    pthread_create(&Students[i], NULL, Student_Activity, (void *)(__intptr_t)i);
  }

  // Simulate the end of office hours after a fixed duration (for testing)
  sleep(10);  // This represents the duration of office hours
  pthread_mutex_lock(&oho_mutex);
  office_hours_over = 1;  // Set office hours to over
  pthread_mutex_unlock(&oho_mutex);
  sem_post(&ta_status);  // Wake up the TA if they are asleep
  printf("OFFICE HOURS ARE OVER.\n");

  // Wait for all student threads to complete
  for (int i = 0; i < number_of_students; i++) {
    pthread_join(Students[i], NULL);
  }

  // Cleanup semaphores and mutexes
  sem_destroy(&ta_status);
  sem_destroy(&ta_ready);

  for (int i = 0; i < 3; i++) {
    sem_destroy(&chair[i]);
  }
  pthread_mutex_destroy(&sw_mutex);
  pthread_mutex_destroy(&oho_mutex);

  // Free allocated memory for student threads
  free(Students);

  return 0;
}
