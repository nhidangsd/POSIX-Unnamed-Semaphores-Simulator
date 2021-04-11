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
  // if (sem_init(&(semData->MutexPtr), 0, 1) == -1) {
  //   fprintf(stderr, "Unable to initialize semaphore\n");
  //   exit(EXT_SEMAPHORE);
  // }
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
                    char* name, int n, SEM_DATA* semData, int* val){
    threadData->Operation = operation;
    threadData->Name = name;
    threadData->N = n;
    threadData->SemPtr = semData;
    threadData->ValuePtr = val;
};


void* operate(void* VoidPtr){
    /* Typecast into a pointer of the expected type. */
    THREAD_DATA	*DataPtr = (THREAD_DATA *) VoidPtr;

    /* Enter Critical Section */
    sem_wait(&(DataPtr->SemPtr->MutexPtr));

   /* Put this thread to sleep for N milliseconds 
    * to simulate the amount of time to consume a product (put a candy in the box)
    */
    sleep(DataPtr->N);

    /* Update the share data after being modified by the this thread */

    /* Clear output stream */
    fflush(stdout);

    /* Exit Critical Section */
    sem_post(&(DataPtr->SemPtr->MutexPtr));
    return NULL;
};

void produce(void* VoidPtr){

};

void consume(void* VoidPtr){

};