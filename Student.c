#include "Student.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "TA_Activity.h"

void *Student_Activity(void *threadID) {
    // Student needs help from the TA
    printf("Student %d needs help from the TA.\n", (int)(__intptr_t)threadID);

    // Lock to check for available chairs
    pthread_mutex_lock(&mutex);

    // Student tries to sit on a chair
    if (StudentsWaiting < 3) {
        printf("Student %d takes a seat.\n", (int)(__intptr_t)threadID);
        StudentsWaiting++;
        printf("S: StudentsWaiting: %d\n", StudentsWaiting);
        // Wake up the TA if student successfully takes a seat
        sem_post(&ta_status);
        printf("Student %d wakes up the TA\n", (int)(__intptr_t)threadID);
        pthread_mutex_unlock(&mutex);  // Unlock before waiting for TA

        // Student waits for the TA's signal to enter the office
        sem_wait(&ta_chair);
        printf("Student %d enters the office.\n", (int)(__intptr_t)threadID);

        // Simulate getting help from the TA
        printf("Student %d is getting help from the TA.\n", (int)(__intptr_t)threadID);
        sleep(1);  // Simulate help time
        printf("Student %d has left the TA's room after getting help.\n", (int)(__intptr_t)threadID);

        pthread_mutex_lock(&office_hours_mutex);
        office_hours_over = 1;
        pthread_mutex_unlock(&office_hours_mutex);

    } else {
        // No chairs available, student leaves
        printf("Student %d found no available chair and will return later.\n", (int)(__intptr_t)threadID);
        pthread_mutex_unlock(&mutex);  // Ensure mutex is unlocked in this path
        pthread_exit(NULL);
    }

    pthread_exit(NULL);
}
