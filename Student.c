#include "Student.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "TA_Activity.h"

void *Student_Activity(void *threadID) {
  // Student needs help from the TA
  printf("Student %d needs help from the TA.\n", (int)(__intptr_t)threadID);

  // Lock to check for available chairs
  pthread_mutex_lock(&mutex);

  // Student tries to sit on a chair
  if (ChairsCount < 3) {
    printf("Student %d takes a seat.\n", (int)(__intptr_t)threadID);
    ChairsCount++;

    // Wake up the TA if student successfully takes a seat
    sem_post(&ta_status);

    // Student is waiting for the TA
    sem_wait(&chair[CurrentIndex]);
  } else {
    // No chairs available, student leaves
    printf("Student %d found no available chair and will return later.\n",
           (int)(__intptr_t)threadID);
    pthread_exit(NULL);
  }

  // Unlock after checking/modifying chairs count
  pthread_mutex_unlock(&mutex);

  // Lock the mutex to safely modify chairs count
  pthread_mutex_lock(&mutex);

  // Student leaves his/her chair
  ChairsCount--;

  // Unlock the mutex after modification
  pthread_mutex_unlock(&mutex);

  // Student is getting help from the TA
  printf("Student %d is getting help from the TA.\n",
         (int)(__intptr_t)threadID);
  //sleep(rand() % 5 + 1);  // Simulate time spent getting help
  sleep(1); //TEST

  // Student leaves the TA room
  printf("Student %d has left the TA's room after getting help.\n",
         (int)(__intptr_t)threadID);

  pthread_exit(NULL);
}
