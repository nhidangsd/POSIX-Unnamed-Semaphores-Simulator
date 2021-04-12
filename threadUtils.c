#include <stdlib.h>
#include <stdio.h>          /* stdout */
#include <unistd.h>
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
    BUFFER_DATA* BufferData = ThreadData->BufferPtr;

    while(!(productionDone(BufferData) && consumptionDone(BufferData)))
    {

      /* Enter Critical Section */
      sem_wait(&(SemData->MutexPtr));

      /* Update the share data after being modified by the this thread */
      switch(ThreadData->Operation){

        case PRODUCE:
          if(!productionDone(BufferData)){
            sem_wait(&(SemData->EmptyPtr));
            produce(ThreadData);
            sem_post(&(SemData->FullPtr));
          }
          break;

        case CONSUME:
          if(!consumptionDone(BufferData)){
            sem_wait(&(SemData->FullPtr));          
            consume(ThreadData);
            sem_post(&(SemData->EmptyPtr));
          }
          break;

        default:
          /* Bad input, return without any operations */
          return NULL;
      }
      
      /* Clear output stream */
      fflush(stdout);

      /* Exit Critical Section */
      sem_post(&(SemData->MutexPtr));
    
    }
  return NULL;
    
};

void produce(THREAD_DATA * ThreadPtr){
  BUFFER_DATA* BufferData = ThreadPtr->BufferPtr;

  /* Put this thread to sleep for N milliseconds 
  * to simulate the amount of time to consume a product (put a candy in the box)
  */
  sleep(ThreadPtr->N);

  /* Update the Num of times this thread has performed action */
  ThreadPtr->Counter++;

  /* Update ProducerCount*/
  BufferData->ProducerCount++;
};

void consume(THREAD_DATA * ThreadPtr){
  BUFFER_DATA* BufferData = ThreadPtr->BufferPtr;

  /* Put this thread to sleep for N milliseconds 
  * to simulate the amount of time to consume a product (put a candy in the box)
  */
  sleep(ThreadPtr->N);

  /* Update the Num of times this thread has performed action */
  ThreadPtr->Counter++;

  /* Update the Consumer Count */
  BufferData->ConsumerCount++;
};


int productionDone(BUFFER_DATA* bufferPtr){
  return (bufferPtr->ProducerCount < 100) ? 0 : 1;
}

int consumptionDone(BUFFER_DATA* bufferPtr){
  return (bufferPtr->ConsumerCount < 100) ? 0 : 1;
}



