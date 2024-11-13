#include "TA_Activity.h"

#include <stdio.h>
#include <unistd.h>

void *TA_Activity() {
  while (1) {
    // Check if office hours are over
    //printf("TA - CHECKING FOR OFFICE HOURS\n");
    pthread_mutex_lock(&oho_mutex);
    if (office_hours_over) {
      pthread_mutex_unlock(&oho_mutex);
      break;  // End the loop if office hours are over
    }
    pthread_mutex_unlock(&oho_mutex);

    // Block TA thread until a student arrives
    if (ta_awake == 0) {
      printf("TA - TA is sleeping\n");
      printf("TA - TA THREAD BLOCKED\n\n");

      sem_wait(&ta_status);
    }

    printf("TA - TA is awake\n");
    printf("TA - students_waiting: %d. \n\n", students_waiting);

    // Check if there are students waiting
    pthread_mutex_lock(&sw_mutex);
    if (students_waiting > 0) {
      // Signal next waiting student to enter the office
      printf("TA - TA SIGNALS the next student from chair: %d.\n",
             next_waiting_index);

      // Free up the specific chair outside for the next student
      //sem_post(&chair[next_waiting_index]);   
      printf("TA - Seat %d is now open.\n", next_waiting_index);

      //printf("TA - NEXT AVAILABLE SEAT: %d\n",available_seat_index);

      // Update number of students waiting
      students_waiting--;
      printf("TA - students_waiting: %d. \n\n", students_waiting);

      // Update next_waiting_index
      next_waiting_index = (next_waiting_index + 1) % 3;
      printf("TA - Next student is at chair %d.\n", next_waiting_index);
      pthread_mutex_unlock(&sw_mutex);

      // TA is currently helping the student
      printf("TA - Door is closed\n\n");
      sleep(2);  // Simulate helping the student
      printf("TA has finished helping the student.\n");

      // Signal that the TA office chair is available for the next student
      sem_post(&ta_chair_ready);
      printf("TA is ready for the next student.\n");
      printf("TA - students_waiting: %d. \n\n", students_waiting);

    } else {
      // No students waiting.
      printf("TA - No students waiting. TA goes back to sleep.\n");
      pthread_mutex_unlock(&sw_mutex);
      break;
    }
  }
  pthread_exit(NULL);
}
