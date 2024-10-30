#ifndef STUDENT_H
#define STUDENT_H

#include <pthread.h>

// Shared declarations
extern pthread_mutex_t mutex;
extern pthread_mutex_t office_hours_mutex;
extern pthread_cond_t office_hours_cond;
extern int ChairsCount;
extern int CurrentIndex;
extern int office_hours_over;

// Function prototype for Student activity
void *Student_Activity(void *threadID);

#endif  // STUDENT_H