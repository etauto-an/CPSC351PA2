#include "Student.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "TA_Activity.h"

void *Student_Activity(void *threadID) {
  // Check if office hours are ongoing
  pthread_mutex_lock(&oho_mutex);
  if (office_hours_over) {
    pthread_mutex_unlock(&oho_mutex);
    printf("S - Office hours are over. Student %d is leaving.\n",
           (int)(__intptr_t)threadID);
    pthread_exit(NULL);
  }
  pthread_mutex_unlock(&oho_mutex);

  // Student needs help from the TA
  printf("S - Student %d needs help from the TA.\n", (int)(__intptr_t)threadID);

  pthread_mutex_lock(&sw_mutex);
  // Student tries to sit on a chair
  if (students_waiting < 3) {
    printf("S - Student %d TAKES SEAT %d.\n", (int)(__intptr_t)threadID,
           available_seat_index);
  
    sem_wait(&chair[available_seat_index]);

    available_seat_index = (available_seat_index + 1) % 3;
    printf("S - NEXT AVAILABLE SEAT AT INDEX: %d\n", available_seat_index);
    students_waiting++;
    printf("S - students_waiting: %d\n\n", students_waiting);

    if (ta_awake == 0) {
      ta_awake = 1;
      sem_post(&ta_status);  // Wake up the TA
      printf("S - Student %d wakes up the TA\n", (int)(__intptr_t)threadID);
    }
    pthread_mutex_unlock(&sw_mutex);

    // Student waits to be called into the TA’s office
    printf("S - STUDENT THREAD IS BLOCKED\n\n");
    sem_wait(&ta_chair_ready);

    // The student now leaves their chair and goes into the TA’s office
    sem_post(&chair[available_seat_index]);

    printf("S - Student %d is getting help from the TA.\n\n",
           (int)(__intptr_t)threadID);

    // Simulate time spent getting help (optional)
    sleep(1);

    // After receiving help, student leaves the TA's office
    printf("S - Student %d has finished and is leaving the TA's office.\n",
           (int)(__intptr_t)threadID);
    printf("S - students_waiting: %d\n\n", students_waiting);

  } else {
    // If no chair is available, student leaves to return later
    printf("S - Student %d found no available chair and will return later.\n\n",
           (int)(__intptr_t)threadID);

    pthread_mutex_unlock(&sw_mutex);  // Correctly unlock sw_mutex in this path
    pthread_exit(NULL);
  }

  pthread_exit(NULL);
}
