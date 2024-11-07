#include "TA_Activity.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *TA_Activity() {
    while (1) {
        // Check if office hours are over
        printf("CHECKING FOR OFFICE HOURS\n");
        pthread_mutex_lock(&oho_mutex);
        if (office_hours_over) {
            pthread_mutex_unlock(&oho_mutex);
            break;  // End the loop if office hours are over
        }
        pthread_mutex_unlock(&oho_mutex);

        // Block TA thread until a student arrives
        printf("TA is sleeping\n");

        printf("TA - students_waiting: %d. \n", students_waiting);
        printf ("\n");

        sem_wait(&ta_status);
        printf("TA is awake\n");

        // Check if there are students waiting
        pthread_mutex_lock(&sw_mutex);
        if (students_waiting > 0) {
            // Signal next waiting student to enter the office
            printf("TA signals the next student from chair: %d.\n",
                   next_waiting_index);

            // Free up the specific chair outside for the next student
            sem_post(&chair[next_waiting_index]);
            printf("Seat %d is now open.\n", next_waiting_index);

            // Update number of students waiting
            students_waiting--;
            printf("TA - students_waiting: %d. \n", students_waiting);
            printf ("\n");

            // Update next_waiting_index
            next_waiting_index = (next_waiting_index + 1) % 3;
            pthread_mutex_unlock(&sw_mutex);

            // TA is currently helping the student
            printf("Door is closed\n");
            sleep(2);  // Simulate helping the student
            printf("TA has finished helping the student.\n");

            // Signal that the TA office chair is available for the next student
            sem_post(&ta_chair_ready);
            printf("TA is ready for the next student.\n");
            printf("TA - students_waiting: %d. \n", students_waiting);
            printf ("\n");

        } else {
            // No students waiting.
            printf("No students waiting. Exiting\n");
            office_hours_over = 1;
            pthread_mutex_unlock(&sw_mutex);
            break;
        }
    }
    printf("OFFICE HOURS ARE OVER.\n");
    pthread_exit(NULL);
}
