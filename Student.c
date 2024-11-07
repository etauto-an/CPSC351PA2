#include "Student.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "TA_Activity.h"

void *Student_Activity(void *threadID) {
    // Check if office hours are ongoing
    pthread_mutex_lock(&oho_mutex);
    if (office_hours_over) {
        pthread_mutex_unlock(&oho_mutex);
        printf("Office hours are over. Student %d is leaving.\n",
               (int)(__intptr_t)threadID);
        pthread_exit(NULL);
    }
    pthread_mutex_unlock(&oho_mutex);

    // Student needs help from the TA
    printf("Student %d needs help from the TA.\n", (int)(__intptr_t)threadID);

    pthread_mutex_lock(&sw_mutex);
    // Student tries to sit on a chair
    if (students_waiting < 3) {
        printf("Student %d takes seat %d.\n", (int)(__intptr_t)threadID, available_seat_index);
        students_waiting++;
        printf("S - students_waiting: %d\n", students_waiting);

        if (ta_awake == 0) {
            ta_awake = 1;
            sem_post(&ta_status);  // Wake up the TA
            printf("Student %d wakes up the TA\n", (int)(__intptr_t)threadID);
        }
        pthread_mutex_unlock(&sw_mutex);

        // Student waits to be called into the TA’s office
        sem_wait(&ta_chair_ready);

        // The student now leaves their chair and goes into the TA’s office
        printf("Student %d is getting help from the TA.\n", (int)(__intptr_t)threadID);

        // Simulate time spent getting help (optional)
        sleep(2);

        // After receiving help, student leaves the TA's office
        printf("Student %d has finished and is leaving the TA's office.\n", (int)(__intptr_t)threadID);

    } else {
        // If no chair is available, student leaves to return later
        printf("Student %d found no available chair and will return later.\n",
               (int)(__intptr_t)threadID);
        pthread_mutex_unlock(&sw_mutex);  // Correctly unlock sw_mutex in this path
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}
