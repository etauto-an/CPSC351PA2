#include "TA_Activity.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *TA_Activity(void *arg) {
  while (1) {
    // TA is currently sleeping.
    sem_wait(&ta_status);

    // Lock to check if office hours are over
    pthread_mutex_lock(&office_hours_mutex);
    if (office_hours_over) {
      pthread_mutex_unlock(&office_hours_mutex);
      break;  // Break the loop and exit the TA thread
    }
    pthread_mutex_unlock(&office_hours_mutex);

    // Lock the shared chairs_count mutex to safely access resources
    pthread_mutex_lock(&mutex);

    // If chairs are empty, continue the loop (no student to help)
    if (ChairsCount == 0) {
      pthread_mutex_unlock(&mutex);
      continue;
    }

    // TA gets next student on chair
    ChairsCount--;
    

    // Unlock the shared resource mutex
    pthread_mutex_unlock(&mutex);

    // Signal the next student to come in
    sem_post(&chair[CurrentIndex]);

    // Update the index to point to the

    // Simulate TA helping the student
    // sleep(rand() % 5 + 1);
    sleep(1);  // TEST next student waiting (using circular
    // queue)
    //CurrentIndex = (CurrentIndex + 1) % 3;

    // TA is ready for the next student
    sem_post(&next_student);
  }

  pthread_exit(NULL);
}