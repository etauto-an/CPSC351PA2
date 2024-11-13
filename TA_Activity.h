#ifndef TA_ACTIVITY_H
#define TA_ACTIVITY_H

#include <pthread.h>
#include <semaphore.h>

// Shared declarations
extern pthread_mutex_t sw_mutex;
extern pthread_mutex_t oho_mutex;

extern sem_t ta_status;
extern sem_t chair[3];
extern sem_t ta_chair_ready;

extern int students_waiting;
extern int next_waiting_index;
extern int available_seat_index;
extern int ta_awake;
extern int office_hours_over;

// Function prototype for TA activity
void *TA_Activity();

#endif  // TA_ACTIVITY_H
