#include "TA_Activity.h"

#include <stdio.h>
#include <unistd.h>

void *TA_Activity() {
  while (1) {
    // Check if office hours are over
    pthread_mutex_lock(&oho_mutex);
    if (office_hours_over) {
      pthread_mutex_unlock(&oho_mutex);
      break;  // End the loop if office hours are over
    }
    pthread_mutex_unlock(&oho_mutex);

    // Wait for a student to signal that they need help
    printf("TA(%d) - TA is sleeping.\n\n", students_waiting);
    printf("TA THREAD BLOCKED\n\n");
    sem_wait(&ta_status);  // TA waits here until a student signal

    // TA has been woken up by a student
    printf("TA(%d) - TA is awake and ready to help.\n", students_waiting);

    // Check if there are students waiting
    pthread_mutex_lock(&sw_mutex);
    if (students_waiting > 0) {
      // Signal next waiting student to enter the office
      printf("TA(%d)-TA SIGNALS the next student from chair: %d.\n",
             students_waiting, next_waiting_index);

      sem_post(&ta_ready);

      // Update number of students waiting and index
      students_waiting--;
      next_waiting_index = (next_waiting_index + 1) % 3;

      pthread_mutex_unlock(&sw_mutex);

      // Help the student (simulate with sleep)
      printf("TA(%d) - TA is helping the student.\n\n", students_waiting);
      sleep(2);

      // Signal that the TA chair is now available
      printf("TA(%d) - Ready for the next student.\n\n", students_waiting);

    } else {
      // No students waiting.
      printf("TA(%d)-No students waiting. TA goes back to sleep\n\n",
             students_waiting);
      pthread_mutex_unlock(&sw_mutex);
      continue;
      ;
    }
  }
  pthread_exit(NULL);
}
