#include <stdlib.h>     /* atoi */
#include <stdio.h>      /* printf */
#include <unistd.h>     /* getopt */
#include <pthread.h>	/* POSIX threads prototypes & defns */
#include "mizzo.h"
#include "threadUtils.h"
#include "testMizzo.h"
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
    // testProcessArgs(flags);

    SEM_DATA    SemData;                                /* critical region semaphore */
    initSemData(&SemData);
    // testInitSemData(SemData);

    SHARE_DATA ShareData = {0, 0, 0, 0, {0}, {0}, {0}};
    // testInitShareData(&ShareData);

    void		*ThreadResultPtr;
    
    PRODUCER_DATA CfbData, EesData; 
    initProducerData(&CfbData, FrogBite, flags->f, &SemData, &ShareData);
    initProducerData(&EesData, Escargot, flags->e, &SemData, &ShareData);
    // testNewProducerData(&CfbData);

    CONSUMER_DATA LucyData, EthelData;
    initConsumerData(&LucyData, Lucy, flags->L, &SemData, &ShareData);
    initConsumerData(&EthelData, Ethel, flags->E, &SemData, &ShareData);
    // testNewConsumerData(&LucyData);

   /*
    *   Create children threads
    */
    pthread_t   LucyThread, EthelThread, CfbThread, EesThread;                  /* thread declarations */
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
    * so we will not be checking the ThreadResultPtr, but they
    * could return something using the same mechanisms that we used 
    * to pass data in to the thread.
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
    int* consumed[ConsumerTypeN];
    consumed[Lucy] = (LucyData.Consumed);
    consumed[Ethel] = (EthelData.Consumed);

    io_production_report(ShareData.Produced, consumed);
    
}