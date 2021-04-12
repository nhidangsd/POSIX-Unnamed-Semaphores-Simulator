#ifndef MIZZO_H
#define MIZZO_H
#include <semaphore.h>	/* POSIX semaphores prototypes & defns */
#include "production.h"
/* program exit codes */
#define EXT_NORMAL	 0
#define EXT_USAGE	 5
#define EXT_SEMAPHORE	10
#define EXT_THREAD	15


/*
* Success and error codes.
* If these are changed, be sure to change human/readable
* message string array in class definition.
*/
typedef enum {
    success,      // operation completed successfully
    nosuchblock,  // bad block index
    badblock,     // cannot access block
    badreadwrite, // unable to complete read or write
    nosynch,      // unable to synchronize buffers with blocks
    badflag,      // cannot regcognize the option argument
} result;


typedef struct{
    int E, L, f, e;
} OPTION_ARGS;

/* Type of operations threads can perform */
typedef enum{
    PRODUCE,
    CONSUME,
} OPERATION;

typedef struct{
    sem_t MutexPtr;     /* pointer to Critical section */
    sem_t EmptyPtr;     /* pointer to Critical section */
    sem_t FullPtr;     /* pointer to Critical section */
} SEM_DATA;

typedef struct{
    int Count;
    int OnBelt[ProductTypeN];
    int Consumed[ConsumerTypeN];
} BUFFER_DATA;
typedef struct{
    OPERATION Operation; /* Specify what should be done */
    char* Name;          /* Name of this thread */
    int N;               /* Number of times to perform action */
    SEM_DATA* SemPtr;
    BUFFER_DATA* BufferPtr;        /* pointer to shared data */
} THREAD_DATA;


/* 
 * processArgs - Scans input arguments and setup the flags for later usage 
 * @param 
 */
void processArgs(int argc, char* argv[], OPTION_ARGS* flags);
void runSimulation(OPTION_ARGS flag);

#endif // MIZZO_H