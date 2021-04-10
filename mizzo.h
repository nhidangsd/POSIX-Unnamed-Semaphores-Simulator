#ifndef MIZZO_H
#define MIZZO_H

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
    INCREMENT,
    DECREMENT,
} OPERATION;

typedef struct{
    OPERATION Operation; /* Specify what should be done */
    char* Name;          /* Name of this thread */
    char* Message;       /* Message to display the current state */
    int N;               /* Number of times to perform action */
    sem_t *MutexPtr;     /* pointer to Critical section */
    int *ValuePtr        /* pointer to shared data */
} THREAD_DATA;

/* 
 * processArgs - Scans input arguments and setup the flags for later usage 
 * @param 
 */
void processArgs(int argc, char* argv[], OPTION_ARGS* flags);
void runSimulation(OPTION_ARGS flag);

#endif // MIZZO_H