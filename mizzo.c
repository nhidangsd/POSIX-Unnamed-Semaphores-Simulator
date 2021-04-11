#include <stdlib.h>     /* atoi */
#include <stdio.h>      /* printf */
#include <unistd.h>     /* getopt */
#include <pthread.h>	/* POSIX threads prototypes & defns */
#include "mizzo.h"
#include "threadUtils.h"
#include "testMizzo.h"

void processArgs(int argc, char* argv[], OPTION_ARGS* flags){
    int Option;
    // only take -E, -L, -f, -e as option args
    // ':' expects a value after that option arg
    char const *optionStr = "E:L:f:e:";

    while( (Option = getopt(argc, argv, optionStr)) != -1){

        /* If the option has an argument, optarg is set to point to the
         * argument associated with the option.
         */
        switch(Option){
            case 'E':
                flags->E = atoi(optarg);
                break;
            case 'L':
                flags->L = atoi(optarg);
                break;
            case 'f':
                flags->f = atoi(optarg);
                break;
            case 'e':
                flags->e = atoi(optarg);   
                break;
            default:
                exit(badflag);
        }   
    }

   /*
    * Once the getopt loop is done, the external variable optind contains
    * a number. This is the first argument of argv to process
    * after all options have been processed.
    * argv[optind] is the next mandatory argument.
    */

    int idx = optind;

    /* If idx < argc, there are mandatory arguments to process */
    if (idx < argc) {
    /* Process positional arguments:
     *argv[idx] argv[idx+1], ..., argv[argc-1]
     */
        printf("argv[idx] = %s\n", argv[idx]);
    }
}


void runSimulation(OPTION_ARGS flags){
    int count = 0;  // num of candies produced

    pthread_t   Ethel, Lucy, Cfb, Ees;                  /* thread declarations */
    THREAD_DATA EthelData, LucyData, CfbData, EesData;  /* thread data */
    SEM_DATA    SemData;                                /* critical region semaphore */
    void		*ThreadResultPtr;
    int Val = 0;

    initSemData(&SemData);
    // testInitSemData(SemData);

    /* Initialize data structures -------------------- */
    initThreadData(&EthelData, INCREMENT, "Ethel", flags.E, &SemData, &Val);
    initThreadData(&LucyData, DECREMENT, "Lucy", flags.L, &SemData, &Val);
    initThreadData(&CfbData, INCREMENT, "Cfb", flags.f, &SemData, &Val);
    initThreadData(&EesData, DECREMENT, "Ees", flags.e, &SemData, &Val);
    
    // testInitThreadData(EthelData);
   /*
    *   Create children threads
    */
    if (pthread_create(&Ethel, NULL, operate, &EthelData)) {
        fprintf(stderr, "Unable to create Ethel thread\n");
        exit(EXT_THREAD);
    }

    if (pthread_create(&Lucy, NULL, operate, &LucyData)) {
        fprintf(stderr, "Unable to create Lucy thread\n");
        exit(EXT_THREAD);
    }
    if (pthread_create(&Cfb, NULL, operate, &CfbData)) {
        fprintf(stderr, "Unable to create Cfb thread\n");
        exit(EXT_THREAD);
    }

    if (pthread_create(&Ees, NULL, operate, &EesData)) {
        fprintf(stderr, "Unable to create Ees thread\n");
        exit(EXT_THREAD);
    }

//    /* wait for threads to exit --------------------
//     * Note that these threads always return a NULL result pointer
//     * so we will not be checking the ThreadResultPtr, but they
//     * could return something using the same mechanisms that we used 
//     * to pass data in to the thread.
//     */

    if (pthread_join(Ethel, &ThreadResultPtr)) {
        fprintf(stderr, "Thread join error\n");
        exit(EXT_THREAD);
    }
  
    if (pthread_join(Lucy, &ThreadResultPtr))  {
        fprintf(stderr, "Thread join error\n");
        exit(EXT_THREAD);
    }
    
    if (pthread_join(Cfb, &ThreadResultPtr)) {
        fprintf(stderr, "Thread join error\n");
        exit(EXT_THREAD);
    }
  
    if (pthread_join(Ees, &ThreadResultPtr))  {
        fprintf(stderr, "Thread join error\n");
        exit(EXT_THREAD);
    }
    printf("Back to Main thread\n");
    
}