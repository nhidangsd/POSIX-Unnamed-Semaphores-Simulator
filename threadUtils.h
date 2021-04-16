#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H
#include "mizzo.h"

void createSemaphore(sem_t* sem, int initalVal);
void initSemData(SEM_DATA* semData);
void initShareData(SHARE_DATA* sharePtr);
void initProducerData(PRODUCER_DATA* threadPtr, ProductType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr);
void initConsumerData(CONSUMER_DATA* threadPtr, ConsumerType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr);


/* produce(void* VoidPtr)
 * - Executes Producer's task.
 * @param VoidPtr: A pointer pointing to PRODUCT_DATA 
 */
void* produce(void* VoidPtr);


/* produce(void* VoidPtr)
 * - Executes Consumer's task.
 * @param VoidPtr: A pointer pointing to CONSUMER_DATA 
 */
void* consume(void* VoidPtr);

ProductType updateIn(PRODUCER_DATA * ThreadPtr);

ProductType updateOut(CONSUMER_DATA * ThreadPtr);

int productionDone(PRODUCER_DATA * ThreadPtr);

int consumptionDone(CONSUMER_DATA * ThreadPtr);

/* hasTooManyCFB(PRODUCER_DATA* ThreadPtr)
 * - Checks if we over produce FrogBite 
 * @param ThreadPtr:  pointer to a producer thread
 */
int hasTooManyCFB(PRODUCER_DATA* ThreadPtr);


/* msleep(long msec)
 * Sleeps for the requested number of milliseconds. 
 * @param msec: the amount of time to sleep in milliseconds.
 */
int msleep(long msec);


#endif // TTHREAD_UTILS_H