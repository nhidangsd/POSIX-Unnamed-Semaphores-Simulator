#include <stdlib.h>
#include <stdio.h>          /* stdout */
#include <errno.h>
#include <time.h>
#include "threadUtils.h"
#include "production.h"

/* createSemaphore(sem_t* sem, int initalVal)
 * - A helper function for initSemData(SEM_DATA* semData)
 * - Initialize a semephore in SEM_DATA
 * @param sem:  A pointer pointing to a Semaphore
 * @param initVal:  An integer containing initial value for that Semaphore
*/
void createSemaphore(sem_t* sem, int initalVal){

   /* Initialize a semaphore --------------------
    * arg 1 - semaphore handle
    * arg 2 - Always zero for unnamed semphores
    * arg 3 - Initial value
    */  
  if (sem_init(sem, 0, initalVal) == -1) {
    fprintf(stderr, "Unable to initialize semaphore\n");
    exit(EXT_SEMAPHORE);
  }
};


/* initSemData(SEM_DATA* semData)
 * - Intializes values for each member of a Semaphore Data
 * @param semData:  A pointer poiting to a SEM_DATA
 */
void initSemData(SEM_DATA* semData){
  createSemaphore(&(semData->Mutex), 1);                  /* for mutual exclusion */
  createSemaphore(&(semData->Empty), CONVEYER_BELT_MAX);  /* number empty buf entries */
  createSemaphore(&(semData->Full), 0);                   /* number full buf entries */
};


/* initShareData(SHARE_DATA* sharePtr)
 * - Intializes values for each member in an instance of SHARE_DATA
 * @param sharePtr:  A pointer poiting to an instance of  SHARE_DATA
 */
void initShareData(SHARE_DATA* sharePtr){
  sharePtr->ConsumerCount = sharePtr->ProducerCount = sharePtr->In = sharePtr->Out = 0;
  int i;
  for(i=0; i<CONVEYER_BELT_MAX; i++){
    sharePtr->ConveyerBelt[i] = 0;
  }
  for(i=0; i<ProductTypeN; i++){
    sharePtr->OnBelt[i] = 0;
  }
  for(i=0; i<ProductTypeN; i++){
    sharePtr->Produced[i] = 0;
  }
}


/* initProducerData(PRODUCER_DATA* threadPtr, ProductType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr)
 * - Intializes values for each member in an instance of PRODUCER_DATA
 * @param threadPtr:  A pointer poiting to an instance of PRODUCER_DATA
 * @param name: The type of producer
 * @param n:    The number of milliseconds N that this thread requires to perform its task
 * @param semPtr:   A pointer pointing to Semaphore Data
 * @param sharePtr: A pointer pointing to Share Data
 */
void initProducerData(PRODUCER_DATA* threadPtr, ProductType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr){

    threadPtr->Name = name;
    threadPtr->N = n;
    threadPtr->SemPtr = semPtr;
    threadPtr->SharePtr = sharePtr;
};


/* initinitConsumerData(CONSUMER_DATA* threadPtr, ProductType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr)
 * - Intializes values for each member in an instance of CONSUMER_DATA
 * @param threadPtr:  A pointer poiting to an instance of CONSUMER_DATA
 * @param name: The type of consumer
 * @param n:    The number of milliseconds N that this thread requires to perform its task
 * @param semPtr:   A pointer pointing to Semaphore Data
 * @param sharePtr: A pointer pointing to Share Data
 */
void initConsumerData(CONSUMER_DATA* threadPtr, ConsumerType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr){

    threadPtr->Name = name;
    threadPtr->N = n;

    int i = 0;
    for(i=0; i<ProductTypeN; i++){
      threadPtr->Consumed[i] = 0;
    }
    threadPtr->SemPtr = semPtr;
    threadPtr->SharePtr = sharePtr;
};


/* productionDone(PRODUCER_DATA * ThreadPtr)
 * - Checks if the production task has been done
 * @param ThreadPtr:    A pointer pointing to PRODUCER_DATA 
 */
int productionDone(PRODUCER_DATA * ThreadPtr){
  SHARE_DATA* shareData = ThreadPtr->SharePtr;
  
  return (shareData->ProducerCount < MAX_NUM_OF_PRODUCT) ? 0 : 1;
}


/* consumptionDone(PRODUCER_DATA * ThreadPtr)
 * - Checks if the consumption task has been done
 * @param ThreadPtr:    A pointer pointing to CONSUMER_DATA_DATA 
 */
int consumptionDone(CONSUMER_DATA * ThreadPtr){
  SHARE_DATA* shareData = ThreadPtr->SharePtr;

  return (shareData->ConsumerCount < MAX_NUM_OF_PRODUCT) ? 0 : 1;
}


/* hasTooManyCFB(PRODUCER_DATA* ThreadPtr)
 * - Checks if we over produce FrogBite 
 * @param ThreadPtr:  pointer to a producer thread
 */
int hasTooManyCFB(PRODUCER_DATA* ThreadPtr){
  int isFrogBiteProducer = (ThreadPtr->Name == FrogBite) ? 1 : 0;
  int FrogBiteCounter = ThreadPtr->SharePtr->OnBelt[FrogBite];

  return (FrogBiteCounter >= MAX_FROGBITE_ON_CONVEYER_BELT && isFrogBiteProducer) ? 1 : 0;
}


/* msleep(long msec)
 * Sleeps for the requested number of milliseconds. 
 * @param msec: the amount of time to sleep in milliseconds.
 */
int msleep(long msec)
{
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}
