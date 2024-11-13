#include <pthread.h>    //Create POSIX threads.
#include <semaphore.h>  //To create semaphores
#include <stdio.h>      //Input Output
#include <stdlib.h>
#include <time.h>    //Wait for a random time.
#include <unistd.h>  //Thread calls sleep for specified number of seconds.

pthread_t *Students;  // N threads running as Students.
pthread_t TA;         // Separate Thread for TA.

int students_waiting = 0;    // Number of waiting students
int next_waiting_index = 0;  // Index of next student in line
int available_seat_index =
    0;  // Index where the next arriving student will wait
int ta_awake = 0;

int office_hours_over = 0;  // 0 = office hours are ongoing
                            // 1 = office hours are over

// A semaphore to control TA's sleep.
sem_t ta_status;  // (0 = ASLEEP | 1 = AWAKE)

// An array of 3 semaphores to control the availability of the chairs outside of
// the office.
sem_t chair[3];  // (0 = OCCUPIED | 1 = AVAILABLE)

// A semaphore to ensure the students enter the office 1 at a time.
sem_t ta_chair_ready;  // (0 = OCCUPIED | 1 = AVAILABLE)

// A mutex to control changes to students_waiting.
pthread_mutex_t sw_mutex;

// A mutex to control changes to office_hours_over
pthread_mutex_t oho_mutex;

// Declared Functions
void *TA_Activity();
void *Student_Activity(void *threadID);

int main(int argc, char *argv[]) {
  // User-defined number of student threads to create. (DEFAULT = 5)
  int number_of_students = 0;
  // int id = 0;  // ID to distinguish between student threads
  srand(time(NULL));

  // Initialize semaphores and mutexes
  sem_init(&ta_status, 0, 0);       // TA is initially asleep
  sem_init(&ta_chair_ready, 0, 1);  // TA chair is initially available
  for (int i = 0; i < 3; i++) {
    sem_init(&chair[i], 0, 1);  // Each chair outside is initially available
  }
  pthread_mutex_init(&sw_mutex, NULL);
  pthread_mutex_init(&oho_mutex, NULL);

  // Capture user input
  if (argc < 2) {
    printf("Number of Students not specified. Using default (5) students.\n");
    number_of_students = 5;
  } else {
    number_of_students = atoi(argv[1]);
    printf("Number of Students specified. Creating %d threads.\n\n",
           number_of_students);
  }

  // Allocate memory for Students
  Students = (pthread_t *)malloc(sizeof(pthread_t) * number_of_students);

  // Creating one TA thread and N Student threads
  pthread_create(&TA, NULL, TA_Activity, NULL);
  for (int i = 0; i < number_of_students; i++) {
    pthread_create(&Students[i], NULL, Student_Activity, (void *)(__intptr_t)i);
  }

  // Simulate end of office hours after a while (for testing purposes)
  sleep(20);  // Simulate office hours duration
  pthread_mutex_lock(&oho_mutex);
  office_hours_over = 1;  // Set office hours to over
  pthread_mutex_unlock(&oho_mutex);
  sem_post(&ta_status);  // Wake up the TA if sleeping
  printf("OFFICE HOURS ARE OVER.\n");

  // Cleanup semaphores and mutexes
  sem_destroy(&ta_status);
  sem_destroy(&ta_chair_ready);
  for (int i = 0; i < 3; i++) {
    sem_destroy(&chair[i]);
  }
  pthread_mutex_destroy(&sw_mutex);
  pthread_mutex_destroy(&oho_mutex);

  // Free allocated memory
  free(Students);

  return 0;
}
