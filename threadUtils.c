#include <stdlib.h>
#include <stdio.h>          /* stdout */
#include <errno.h>
#include <time.h>
#include <pthread.h>    /* pthread_exit() */
#include "threadUtils.h"
#include "production.h"
#include "io.h"
#include "testMizzo.h"

void initSemData(SEM_DATA* semData){
  createSemaphore(&(semData->Mutex), 1);
  createSemaphore(&(semData->Empty), 10);
  createSemaphore(&(semData->Full), 0);
};

void createSemaphore(sem_t* sem, int initalVal){

   /* Create the semaphore --------------------
    * arg 1 - semaphore handle
    * arg 2 - Always zero for unnamed semphores
    * arg 3 - Initial value
    */  
  if (sem_init(sem, 0, initalVal) == -1) {
    fprintf(stderr, "Unable to initialize semaphore\n");
    exit(EXT_SEMAPHORE);
  }
};

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


void initProducerData(PRODUCER_DATA* threadPtr, ProductType name, int n, SEM_DATA* semPtr, SHARE_DATA* sharePtr){

    threadPtr->Name = name;
    threadPtr->N = n;
    threadPtr->SemPtr = semPtr;
    threadPtr->SharePtr = sharePtr;
};

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


void* produce(void* VoidPtr){

  /* Typecast into a pointer of the expected type. */
  PRODUCER_DATA	*ThreadPtr = (PRODUCER_DATA *) VoidPtr;
  SEM_DATA* SemPtr = ThreadPtr->SemPtr;
  SHARE_DATA* ShareData = ThreadPtr->SharePtr;

  while (!productionDone(ThreadPtr)){
    // testProductionDone(ThreadPtr);
    /* Put this thread to sleep for N milliseconds 
    * to simulate the amount of time to produce a product
    */ 
    msleep(ThreadPtr->N);
    if(productionDone(ThreadPtr)){
      return NULL;
    }
    
    /* Check if we over produce FrogBite */
    if (!hasTooManyCFB(ThreadPtr)){

      /* Enters Critical Section */
      sem_wait(&(SemPtr->Empty));
      sem_wait(&(SemPtr->Mutex));
    
      /* Updates Conveyer Belt */
      ProductType producer = updateIn(ThreadPtr);
      // testUpdateIn(ThreadPtr, producer);

      /* Show that an item has been added to the belt and 
       * print the current status of the candy factory production.
       */
      io_add_type(producer, ShareData->OnBelt, ShareData->Produced);

      /* Clear output stream */
      fflush(stdout);

      /* Exit Critical Section */
      sem_post(&(SemPtr->Mutex));
      sem_post(&(SemPtr->Full));
      
    }

  }
  return NULL;
};


void* consume(void* VoidPtr){

  /* Typecast into a pointer of the expected type. */
  CONSUMER_DATA	*ThreadPtr = (CONSUMER_DATA *) VoidPtr;
  SEM_DATA* SemPtr = ThreadPtr->SemPtr;
  SHARE_DATA* ShareData = ThreadPtr->SharePtr;

  while (!consumptionDone(ThreadPtr)){
    // testConsumptionDone(ThreadPtr);
    /* Put this thread to sleep for N milliseconds 
    * to simulate the amount of time to consume a product (put a candy in the box)
    */ 
    msleep(ThreadPtr->N);
    if(consumptionDone(ThreadPtr)){
      return NULL;
    }
    /* Enter Critical Section */
    sem_wait(&(SemPtr->Full)); 
    sem_wait(&(SemPtr->Mutex));
 
    /* Updates Conveyer Belt */
    ProductType product = updateOut(ThreadPtr); 
    // testUpdateOut(ThreadPtr, product); 

    /* Show that an item has been removed from the belt and 
     * print the current status of the candy factory production.
    */
    io_remove_type(ThreadPtr->Name, product, ShareData->OnBelt, ThreadPtr->Consumed);

    /* Clear output stream */
    fflush(stdout);

    /* Exit Critical Section */
    sem_post(&(SemPtr->Mutex));
    sem_post(&(SemPtr->Empty));
    
  }
  return NULL;
};

ProductType updateIn(PRODUCER_DATA * ThreadPtr){
  SHARE_DATA* ShareData = ThreadPtr->SharePtr;

  /* Saves the product type that this thread is about to produce */
  ProductType product = ThreadPtr->Name;

  /* Puts the product on the Conveyer Belt & Moves pointer In 1 cell to the right */
  ShareData->ConveyerBelt[ShareData->In] = product;
  ShareData->In = ((ShareData->In +1 ) % CONVEYER_BELT_MAX);

  /* Increments the Num of products have been produced */
  ShareData->ProducerCount++;

  /* Increments the num of products that this thread has been produced */
  ShareData->Produced[product]++; 
  
  /* Increments product's quantity on the Conveyer Belt after Production */
  ShareData->OnBelt[product]++;

  return product;
}

ProductType updateOut(CONSUMER_DATA * ThreadPtr){
  SHARE_DATA* ShareData = ThreadPtr->SharePtr;

  /* Saves the product type that this thread is about to consume */
  ProductType product = ShareData->ConveyerBelt[ShareData->Out];

  /* Moves pointer Out 1 cell to the right after consuming that product */
  ShareData->Out = ((ShareData->Out +1 ) % CONVEYER_BELT_MAX);

  /* Incremnets the Num of Products that have been consumed */
  ShareData->ConsumerCount++;

  /* Incremnets the num of products of this Product type that this thread has been consumed */
  ThreadPtr->Consumed[product]++; 

  /* Decrements product's quantity of this Product Type on the Conveyer Belt after Consumption */
  ShareData->OnBelt[product]--;

  return product;
}


int productionDone(PRODUCER_DATA * ThreadPtr){
  SHARE_DATA* shareData = ThreadPtr->SharePtr;
  
  return (shareData->ProducerCount < MAX_NUM_OF_PRODUCT) ? 0 : 1;
}


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
