#include "TA_Activity.h"

#include <stdio.h>
#include <stdlib.h>
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

    // Block TA thread until a student arrives
		printf("TA IS SLEEPING\n");
		sem_wait(&ta_status);
		printf("TA IS AWAKE \n");

		// Help next student in line
		pthread_mutex_lock(&sw_mutex);
		if (students_waiting > 0) {
			// Signal next waiting student to enter the office
			printf("TA signals next student to enter from chair %d.\n",
			 next_waiting_index);

			// Free up the specific chair outside for the next student
			sem_post(&chair[next_waiting_index]);
			printf("Seat %d is now open.\n", next_waiting_index);

			// Make office chair available for the entering student
			sem_post(&ta_chair_ready);
			printf("DOOR IS OPEN\n");

			// Update number of students waiting
			students_waiting--;
			printf("TA - students_waiting: %d. \n", students_waiting);

			// Update next_waiting_index
			next_waiting_index = (next_waiting_index + 1) % 3;

		} else {
			// No students waiting, TA can exit or go back to sleep
			printf("No students waiting. TA goes back to sleep or exits.\n");
			pthread_mutex_unlock(&sw_mutex);
			continue;  // End loop if no students are waiting
		}
		pthread_mutex_unlock(&sw_mutex);

  	//TA is currently helping the student
		printf("DOOR IS CLOSED\n");
		
		sleep(1);
		printf("TA is finished helping the student.\n");
		sem_post(&ta_chair_ready); // TA office chair now available
		printf("TA READY FOR NEXT STUDENT.\n");

	}
  printf("OFFICE HOURS ARE OVER. \n");
  pthread_exit(NULL);
}
