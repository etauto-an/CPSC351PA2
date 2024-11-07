#ifndef STUDENT_H
#define STUDENT_H

#include <pthread.h>

// Shared declarations
extern pthread_mutex_t sw_mutex;
extern pthread_mutex_t oho_mutex;

// Function prototype for Student activity
void *Student_Activity(void *threadID);

#endif  // STUDENT_H