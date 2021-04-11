#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H
#include "mizzo.h"

void initThreadData(THREAD_DATA* threadData, OPERATION operation, char* name, char* message, int n, sem_t* mutex, int* val);
void* operate(void* VoidPtr);
#endif // TTHREAD_UTILS_H