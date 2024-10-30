#ifndef TA_ACTIVITY_H
#define TA_ACTIVITY_H

#include <pthread.h>
#include <semaphore.h>

// Shared declarations
extern pthread_mutex_t mutex;
extern pthread_mutex_t office_hours_mutex;
extern pthread_cond_t office_hours_cond;
extern int ChairsCount;
extern int CurrentIndex;
extern int office_hours_over;
extern sem_t ta_status;
extern sem_t chair[3];
extern sem_t next_student;

// Function prototype for TA activity
void *TA_Activity(void *arg);

#endif  // TA_ACTIVITY_H
