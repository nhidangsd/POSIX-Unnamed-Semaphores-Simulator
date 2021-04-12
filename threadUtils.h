#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H
#include "mizzo.h"

void initSemData(SEM_DATA* semData);
void initSem(sem_t* sem, int initalVal);
void initThreadData(THREAD_DATA* threadData, OPERATION operation, 
                    char* name, int n, SEM_DATA* semData, BUFFER_DATA* buffer);


void* operate(void* VoidPtr);

// Function to produce an item and
// add it to the buffer
void produce(THREAD_DATA * ThreadPtr);

// Function to consume an item and
// remove it from buffer
void consume(THREAD_DATA * ThreadPtr);

int productionDone(BUFFER_DATA* bufferPtr);

int consumptionDone(BUFFER_DATA* bufferPtr);

#endif // TTHREAD_UTILS_H