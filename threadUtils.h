#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H
#include "mizzo.h"

void initSemData(SEM_DATA* semData);
void createSemaphore(sem_t* sem, int initalVal);
void printConveyerBelt(int OnBelt[], int in, int out);
void initProducerData(PRODUCER_DATA* threadPtr, ProductType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr);
void initConsumerData(CONSUMER_DATA* threadPtr, ConsumerType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr);
// void initBufferData(BUFFER_DATA* buffer);


// Function to produce an item and
// add it to the buffer
void* produce(void* VoidPtr);

// Function to consume an item and
// remove it from buffer
void* consume(void* VoidPtr);

ProductType updateIn(PRODUCER_DATA * ThreadPtr);

ProductType updateOut(CONSUMER_DATA * ThreadPtr);

int productionDone(PRODUCER_DATA * ThreadPtr);

int consumptionDone(CONSUMER_DATA * ThreadPtr);

int hasTooManyCFB(PRODUCER_DATA* ThreadPtr);

int msleep(long tms);


#endif // TTHREAD_UTILS_H