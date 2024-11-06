#include "TA_Activity.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *TA_Activity(void *arg) {
    while (1) {
        // Check if office hours are over
        pthread_mutex_lock(&office_hours_mutex);
        if (office_hours_over) {
            pthread_mutex_unlock(&office_hours_mutex);
            break;  // End the loop if office hours are over
        }
        pthread_mutex_unlock(&office_hours_mutex);

        // TA is currently sleeping, waits for a student to wake them up
        printf("TA IS SLEEPING\n");
        sem_wait(&ta_status);  // Wait for a student to signal the TA

        /****WAITING FOR STUDENT TO CALL sem_post(&ta_status)****/

        printf("TA IS AWAKE\n");

        // Lock the shared chairs_count mutex to safely access resources
        pthread_mutex_lock(&mutex);

        // Check if there are students waiting
        if (StudentsWaiting > 0) {
            StudentsWaiting--;  // Decrement chairs count as TA is helping a student

            // Signal the student to enter the office
            sem_post(&ta_chair);
            printf("TA signals a student to enter the office.\n");
            printf("TA: StudentsWaiting is now %d.\n", StudentsWaiting);
        } else {
            pthread_mutex_unlock(&mutex);
            break;  // End the TA loop if no students are waiting
        }

        pthread_mutex_unlock(&mutex);

        // Simulate helping the student
        sleep(1);  // Simulate time spent helping
        printf("TA finished helping a student and is ready for the next one.\n");
    }

    printf("Office hours over. \n");
    pthread_exit(NULL);
}
