#include <stdlib.h>     /* atol */
#include <stdio.h>      /* printf */
#include <unistd.h>     /* getopt */
#include <pthread.h>	/* POSIX threads prototypes & defns */
#include "mizzo.h"
#include "threadUtils.h"
#include "io.h"


/* processArgs(int argc, char* argv[], OPTION_ARGS* flags)
 * - Scans input arguments and setup the flags for later usage
 * @param argc :    An integer containing the number of entries in argv: 0 to agrc-1
 * @param argv :    Contains pointers t the strings. Note that the program name is argv[0]
 * @param flags:    A struct containing data associated with each Option arguments
 */
void processArgs(int argc, char* argv[], OPTION_ARGS* flags){
    int Option;

    /* only take -E, -L, -f, -e as option args
     *':' expects a value after that option arg
     */
    char const *optionStr = "E:L:f:e:";

    while( (Option = getopt(argc, argv, optionStr)) != -1){

        /* 
         * If the option has an argument, optarg is set to point to the
         * argument associated with the option.
         * In our case, optarg will be the number of milliseconds N 
         * that a specified thread requires to perform its task.
         */
        switch(Option){
            case 'E':
                flags->E = atol(optarg);
                break;
            case 'L':
                flags->L = atol(optarg);
                break;
            case 'f':
                flags->f = atol(optarg);
                break;
            case 'e':
                flags->e = atol(optarg);   
                break;
            default:
                exit(badflag);
        }   
    }
}


/* runSimulation(OPTION_ARGS* flags)
 * - Executes the Producer-Consumer Problem
 * @param flags:    A struct containing data associated with each Option arguments
 */
void runSimulation(OPTION_ARGS* flags){


/*
 * Declare and Intinialize al needed data
*/

    /* Critical region semaphores */
    SEM_DATA    SemData;                               
    initSemData(&SemData);

    /* Declare & Initialize Share Data to use between all threads */
    SHARE_DATA ShareData;
    initShareData(&ShareData);

    /* Declare & Initialize Data for Producer Threads */
    PRODUCER_DATA CfbData, EesData; 
    initProducerData(&CfbData, FrogBite, flags->f, &SemData, &ShareData);
    initProducerData(&EesData, Escargot, flags->e, &SemData, &ShareData);

    /* Declare & Initialize Data for Consumer Threads */
    CONSUMER_DATA LucyData, EthelData;
    initConsumerData(&LucyData, Lucy, flags->L, &SemData, &ShareData);
    initConsumerData(&EthelData, Ethel, flags->E, &SemData, &ShareData);


/* 
 * Create all Children Threads and execute the tasks
 */

    pthread_t   LucyThread, EthelThread, CfbThread, EesThread;    // Threads declaration
    void *ThreadResultPtr;  // Pointer containing the Thread result

    if (pthread_create(&CfbThread, NULL, produce, &CfbData)) {
        fprintf(stderr, "Unable to create Cfb thread\n");
        exit(EXT_THREAD);
    }

    if (pthread_create(&EesThread, NULL, produce, &EesData)) {
        fprintf(stderr, "Unable to create Ees thread\n");
        exit(EXT_THREAD);
    }

    if (pthread_create(&LucyThread, NULL, consume, &LucyData)) {
        fprintf(stderr, "Unable to create Lucy thread\n");
        exit(EXT_THREAD);
    }

    if (pthread_create(&EthelThread, NULL, consume, &EthelData)) {
        fprintf(stderr, "Unable to create Ethel thread\n");
        exit(EXT_THREAD);
    }


    /* wait for threads to exit --------------------
     * Note that these threads always return a NULL result pointer
     * so we will not be checking the ThreadResultPtr.
     */
    if (pthread_join(CfbThread, &ThreadResultPtr)) {
        fprintf(stderr, "Thread join error\n");
        exit(EXT_THREAD);
    }
  
    if (pthread_join(EesThread, &ThreadResultPtr))  {
        fprintf(stderr, "Thread join error\n");
        exit(EXT_THREAD);
    }

    if (pthread_join(LucyThread, &ThreadResultPtr))  {
        fprintf(stderr, "Thread join error\n");
        exit(EXT_THREAD);
    }

    if (pthread_join(EthelThread, &ThreadResultPtr)) {
        fprintf(stderr, "Thread join error\n");
        exit(EXT_THREAD);
    }

    /* Create an array of pointers to consumed arrays for each consumer */
    int* consumed[ConsumerTypeN];
    consumed[Lucy] = LucyData.Consumed;
    consumed[Ethel] = EthelData.Consumed;

    /* Show how many candies of each type produced.  Show how many candies consumed by each consumer. */
    io_production_report(ShareData.Produced, consumed);
    
}


/* produce(void* VoidPtr)
 * - Executes Producer's task.
 * @param VoidPtr: A pointer pointing to PRODUCER_DATA 
 */
void* produce(void* VoidPtr){

  /* Typecast into a pointer of the expected type. */
  PRODUCER_DATA	*ThreadPtr = (PRODUCER_DATA *) VoidPtr;
  SEM_DATA* SemPtr = ThreadPtr->SemPtr;
  SHARE_DATA* ShareData = ThreadPtr->SharePtr;

  /* Keep producing until production task has been finished */
  while (!productionDone(ThreadPtr)){
   
    /* Put this thread to sleep for N milliseconds 
    * to simulate the amount of time to produce a product
    */ 
    msleep(ThreadPtr->N);

    /* Stop the production if Production Task has been finished by other thread */
    if(productionDone(ThreadPtr)){
      return NULL;
    }

    /* Check if we over produce FrogBite */
    if (!hasTooManyCFB(ThreadPtr)){

      /* Wait until there is at least 1 empty entry on he Conveyer Belt to put the producta on */
      sem_wait(&(SemPtr->Empty));
      
      /* Only 1 thread should modify the Share Data at any time */
      sem_wait(&(SemPtr->Mutex));
    
      /* Updates Conveyer Belt */
      ProductType producer = updateIn(ThreadPtr);

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


/* consume(void* VoidPtr)
 * - Executes Consumer's task.
 * @param VoidPtr: A pointer pointing to CONSUMER_DATA 
 */
void* consume(void* VoidPtr){

  /* Typecast into a pointer of the expected type. */
  CONSUMER_DATA	*ThreadPtr = (CONSUMER_DATA *) VoidPtr;
  SEM_DATA* SemPtr = ThreadPtr->SemPtr;
  SHARE_DATA* ShareData = ThreadPtr->SharePtr;

  /* Keep consuming until consumption task has been finished */
  while (!consumptionDone(ThreadPtr)){

    /* Put this thread to sleep for N milliseconds 
    * to simulate the amount of time to consume a product (put a candy in the box)
    */ 
    msleep(ThreadPtr->N);

    /* Stops the consumption if Consumption Task has been finished by other thread */
    if(consumptionDone(ThreadPtr)){
      return NULL;
    }

    /* Wait until there is at least 1 product on the Conveyer Belt to consume */
    sem_wait(&(SemPtr->Full)); 

    /* Only 1 thread should modify the Share Data at any time */
    sem_wait(&(SemPtr->Mutex));
 
    /* Updates Conveyer Belt */
    ProductType product = updateOut(ThreadPtr); 

    /* Show that an item has been removed from the belt and 
     * print the current status of the candy factory production.
    */
    io_remove_type(ThreadPtr->Name, product, ShareData->OnBelt, ThreadPtr->Consumed);

    /* Clear output stream */
    fflush(stdout);

    /* Exit Critical Section after modifying the Share Data */
    sem_post(&(SemPtr->Mutex));
    sem_post(&(SemPtr->Empty));
  }
  return NULL;
};


/* updateIn(PRODUCER_DATA * ThreadPtr)
 * - Adds product to the buffer
 * @param ThreadPtr:    A pointer pointing to PRODUCER_DATA 
 */
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


/* updateOut(PRODUCER_DATA * ThreadPtr)
 * - Removes product from the buffer
 * @param ThreadPtr:    A pointer pointing to CONSUMMER_DATA 
 */
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
