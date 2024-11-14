#include "Student.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "TA_Activity.h"

void *Student_Activity(void *threadID) {
  int id = (int)(__intptr_t)threadID;  // Get student ID from thread argument
  sleep(id);
  // Announce that the student needs help
  printf("S(%d) - Student %d needs help.\n", students_waiting, id);

  pthread_mutex_lock(&sw_mutex);  // Lock to modify shared variables

  // Check if there's an available chair in the waiting area
  if (students_waiting < 3) {  // If less than 3 students are waiting
    // The student takes a seat
    students_waiting++;
    printf("S(%d) - Student %d is waiting on chair %d.\n", students_waiting, id,
           available_seat_index);

    // Occupy the current available seat and update the `available_seat_index`
    sem_wait(&chair[available_seat_index]);
    available_seat_index = (available_seat_index + 1) % 3;

    // Signal the TA that a student is waiting
    sem_post(&ta_status);  // Wake up the TA if they are asleep

    pthread_mutex_unlock(&sw_mutex);  // Unlock after updating shared variables

    // Student waits to be called into the TA’s office
    printf(
        "S(%d) - Student %d is waiting to be called into the TA's office.\n\n",
        students_waiting, id);

    //printf("STUDENT %d THREAD BLOCKED\n\n", id);

    sem_wait(&ta_ready);  // Wait until the TA signals that they are ready
                          // to help this student

    // Free up the seat that the student was sitting in, allowing another
    // student to use it
    sem_post(&chair[next_waiting_index]);

    // Simulate time spent in the TA’s office
    sleep(1);  // Adjust sleep duration as needed for testing

    // The student has finished and leaves the TA's office
    printf(
        "S(%d) - Student %d leaves after getting help.\n\n",
        students_waiting, id);

  } else {
    // No available chairs, so the student leaves to try again later
    printf(
        "S(%d) - Student %d will return later as no chairs are available \n\n",
        students_waiting, id);

    // Unlock the mutex in this path as well
    pthread_mutex_unlock(&sw_mutex);
    pthread_exit(NULL);  // End this thread as the student is leaving
  }

  pthread_exit(NULL);  // End the student thread after receiving help
}
