#ifndef THREAD_UTILS_H
#define THREAD_UTILS_H
#include "mizzo.h"

/* createSemaphore(sem_t* sem, int initalVal)
 * - A helper function for initSemData(SEM_DATA* semData)
 * - Initialize a semephore in SEM_DATA
 * @param sem:  A pointer pointing to a Semaphore
 * @param initVal:  An integer containing initial value for that Semaphore
*/
void createSemaphore(sem_t* sem, int initalVal);


/* initSemData(SEM_DATA* semData)
 * - Intializes values for each member in an instance of SEM_DATA
 * @param semData:  A pointer poiting to an instance of SEM_DATA
 */
void initSemData(SEM_DATA* semData);


/* initShareData(SHARE_DATA* sharePtr)
 * - Intializes values for each member in an instance of SHARE_DATA
 * @param sharePtr:  A pointer poiting to an instance of  SHARE_DATA
 */
void initShareData(SHARE_DATA* sharePtr);


/* initProducerData(PRODUCER_DATA* threadPtr, ProductType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr)
 * - Intializes values for each member in an instance of PRODUCER_DATA
 * @param threadPtr:  A pointer poiting to an instance of PRODUCER_DATA
 * @param name: The type of producer
 * @param n:    The number of milliseconds N that this thread requires to perform its task
 * @param semPtr:   A pointer pointing to Semaphore Data
 * @param sharePtr: A pointer pointing to Share Data
 */
void initProducerData(PRODUCER_DATA* threadPtr, ProductType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr);


/* initinitConsumerData(CONSUMER_DATA* threadPtr, ProductType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr)
 * - Intializes values for each member in an instance of CONSUMER_DATA
 * @param threadPtr:  A pointer poiting to an instance of CONSUMER_DATA
 * @param name: The type of consumer
 * @param n:    The number of milliseconds N that this thread requires to perform its task
 * @param semPtr:   A pointer pointing to Semaphore Data
 * @param sharePtr: A pointer pointing to Share Data
 */
void initConsumerData(CONSUMER_DATA* threadPtr, ConsumerType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr);


/* productionDone(PRODUCER_DATA * ThreadPtr)
 * - Checks if the production task has been done
 * @param ThreadPtr:    A pointer pointing to PRODUCER_DATA 
 */
int productionDone(PRODUCER_DATA * ThreadPtr);


/* consumptionDone(PRODUCER_DATA * ThreadPtr)
 * - Checks if the consumption task has been done
 * @param ThreadPtr:    A pointer pointing to CONSUMER_DATA_DATA 
 */
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