#include <stdlib.h>
#include <stdio.h>          /* stdout */
#include <unistd.h>
#include <pthread.h>    /* pthread_exit() */
#include "threadUtils.h"
#include "production.h"
#include "io.h"

void initSemData(SEM_DATA* semData){
  initSem(&(semData->MutexPtr), 1);
  initSem(&(semData->EmptyPtr), 10);
  initSem(&(semData->FullPtr), 0);
};

void initSem(sem_t* sem, int initalVal){

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

void initThreadData(THREAD_DATA* threadData, OPERATION operation, 
                    char* name, int n, SEM_DATA* semData, BUFFER_DATA* buffer){
    threadData->Operation = operation;
    threadData->Name = name;
    threadData->N = n;
    threadData->Counter = 0;
    threadData->SemPtr = semData;
    threadData->BufferPtr = buffer;
};


void* operate(void* VoidPtr){

  /* Typecast into a pointer of the expected type. */
  THREAD_DATA	*ThreadData = (THREAD_DATA *) VoidPtr;
  SEM_DATA* SemData = ThreadData->SemPtr;
  printf("%s calls function operate()\n", ThreadData->Name);

  /* Enter Critical Section */
  sem_wait(&(SemData->MutexPtr));

  
  /* Update the share data after being modified by the this thread */
  switch(ThreadData->Operation){

    case PRODUCE:
      produce(ThreadData);
      break;

    case CONSUME:
      consume(ThreadData);
      break;

    default:
      /* Bad input, return without any operations */
      return NULL;
  }
      
  /* Clear output stream */
  fflush(stdout);

  /* Exit Critical Section */
  sem_post(&(SemData->MutexPtr));
    
  return NULL;
    
};

void* produce(void* VoidPtr){
  
  /* Typecast into a pointer of the expected type. */
  THREAD_DATA	*ThreadPtr = (THREAD_DATA *) VoidPtr;
  SEM_DATA* SemPtr = ThreadPtr->SemPtr;
  BUFFER_DATA* BufferPtr = ThreadPtr->BufferPtr;

  while (1){
    sem_wait(&(SemPtr->EmptyPtr));

    /* Enter Critical Section */
    sem_wait(&(SemPtr->MutexPtr));

    /* Put this thread to sleep for N milliseconds 
    * to simulate the amount of time to produce a product
    */
    printf("%s is producing\n", ThreadPtr->Name);
    sleep(ThreadPtr->N);

    /* Update the Num of times this thread has performed action */
    ThreadPtr->Counter++;

    /* Update ProducerCount*/
    BufferPtr->ProducerCount++;
    printf("%s updates ProducerCount to %d \n", ThreadPtr->Name, BufferPtr->ProducerCount);
    
    /* Exit Critical Section */
    sem_post(&(SemPtr->MutexPtr));
    sem_post(&(SemPtr->FullPtr));

    testProductionDone(ThreadPtr, productionDone(ThreadPtr));
    if(productionDone(ThreadPtr) ){
      printf("Exiting: producer\n");
      pthread_exit(0);
    }
  }

  return NULL;
  
};

void* consume(void* VoidPtr){

  /* Typecast into a pointer of the expected type. */
  THREAD_DATA	*ThreadPtr = (THREAD_DATA *) VoidPtr;
  SEM_DATA* SemPtr = ThreadPtr->SemPtr;
  BUFFER_DATA* BufferPtr = ThreadPtr->BufferPtr;

  while (1){
    sem_wait(&(SemPtr->FullPtr)); 

    /* Enter Critical Section */
    sem_wait(&(SemPtr->MutexPtr));

    /* Put this thread to sleep for N milliseconds 
    * to simulate the amount of time to consume a product (put a candy in the box)
    */ 
    printf("%s is consuming\n", ThreadPtr->Name);  
    sleep(ThreadPtr->N);

    /* Update the Num of times this thread has performed action */
    ThreadPtr->Counter++;

    /* Update the Consumer Count */
    BufferPtr->ConsumerCount++;
    printf("%s updates ConsumerCount to %d \n", ThreadPtr->Name, BufferPtr->ConsumerCount);   
    
    /* Exit Critical Section */
    sem_post(&(SemPtr->MutexPtr));
    sem_post(&(SemPtr->EmptyPtr));

    testConsumptionDone(ThreadPtr, consumptionDone(ThreadPtr));
    if( consumptionDone(ThreadPtr) ){
      printf("Exiting: consumer\n");
      pthread_exit(0);
    }
  }
  return NULL;  
};

void updateIn(THREAD_DATA * ThreadPtr){


}

int productionDone(THREAD_DATA * ThreadPtr){
  BUFFER_DATA* bufferPtr = ThreadPtr->BufferPtr;
  printf("%s checks if productionDOne: res = %d \n", ThreadPtr-> Name, (bufferPtr->ProducerCount < MAX_NUM_OF_PRODUCT) ? 0 : 1);
  return (bufferPtr->ProducerCount < MAX_NUM_OF_PRODUCT) ? 0 : 1;
}

int consumptionDone(THREAD_DATA * ThreadPtr){
  BUFFER_DATA* bufferPtr = ThreadPtr->BufferPtr;
  printf("%s checks if consumptionDone: res = %d \n", ThreadPtr->Name , (bufferPtr->ConsumerCount < MAX_NUM_OF_PRODUCT) ? 0 : 1);
  return (bufferPtr->ConsumerCount < MAX_NUM_OF_PRODUCT) ? 0 : 1;
}




