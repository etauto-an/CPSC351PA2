#include "Student.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "TA_Activity.h"

void *Student_Activity(void *threadID) {
  /* TODO

  // Student needs help from the TA
  // Student tries to sit on a chair.
  // wake up the TA.
  // lock

  // unlock
  // Student leaves his/her chair.
  // Student is getting help from the TA
  // Student waits to go next.
  // Student left TA room

  // If student didn't find any chair to sit on.
  // Student will return at another time

  // hint: use sem_wait(); sem_post(); pthread_mutex_lock();
  pthread_mutex_unlock()
  */
  pthread_exit(NULL);
}
