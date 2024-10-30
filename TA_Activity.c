#include "TA_Activity.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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
