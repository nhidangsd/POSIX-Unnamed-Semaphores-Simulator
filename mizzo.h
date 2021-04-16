#ifndef MIZZO_H
#define MIZZO_H
#include <semaphore.h>	/* POSIX semaphores prototypes & defns */
#include "production.h" /* ProductType and ConsumerType */

/* program exit codes */
#define EXT_SEMAPHORE	10
#define EXT_THREAD	15

/* Maximum quantity of product that needs to be produced */
#define MAX_NUM_OF_PRODUCT 100

/* Maximum quantity of product that can be placed on the Conveyer Belt */
#define CONVEYER_BELT_MAX 10

/* Maximum quantity of Crunchy Frog Bite that can be placed on the Conveyer Belt */
#define MAX_FROGBITE_ON_CONVEYER_BELT   3

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


/* struct OPTION_ARGS 
 * Contains data associated with each Option arguments 
 */
typedef struct{
    int E, L, f, e;
} OPTION_ARGS;


/* struct SEM_DATA 
 * Contains all semaphores data 
 */
typedef struct{
    sem_t Mutex;    
    sem_t Empty;   
    sem_t Full;   
} SEM_DATA;


/* struct SHARE_DATA 
 * Contains sharing data between all threads
 */
typedef struct{
    int ConsumerCount;
    int ProducerCount;

    int In, Out;
    /* Array containing products that are placed on the Conveyer Belt */
    int ConveyerBelt[CONVEYER_BELT_MAX];
    /* Array of number of candies of each type that 
     * are on the belt and have not yet been consumed 
     */
    int OnBelt[ProductTypeN]; 
    int Produced[ProductTypeN];   /* Array of number of products of each type that have been produced */
} SHARE_DATA;


/* struct CONSUMER_DATA 
 * Contains data for Consumer Thread
 */
typedef struct{
    ConsumerType Name;          /* Consumer type */
    int N;                      /* Number of milliseconds N needed to perform task  */
    int Consumed[ProductTypeN]; /* Array of number of products of each type that have been consumed.*/
    SEM_DATA* SemPtr;           /* Pointer to all Semaphore data */
    SHARE_DATA* SharePtr;       /* Pointer to shared data */
} CONSUMER_DATA;


/* struct PRODUCER_DATA 
 * Contains data for Producer Thread
 */
typedef struct{
    ProductType Name;           /* Product type */
    int N;                      /* Number of milliseconds N needed to perform task  */
    SEM_DATA* SemPtr;           /* Pointer to all Semaphore data */
    SHARE_DATA* SharePtr;       /* Pointer to shared data */
} PRODUCER_DATA;


/* processArgs(int argc, char* argv[], OPTION_ARGS* flags)
 * - Scans input arguments and setup the flags for later usage
 * @param argc :    An integer containing the number of entries in argv: 0 to agrc-1
 * @param argv :    Contains pointers t the strings. Note that the program name is argv[0]
 * @param flags:    A struct containing data associated with each Option arguments
 */
void processArgs(int argc, char* argv[], OPTION_ARGS* flags);


/* runSimulation(OPTION_ARGS* flags)
 * - Executes the Producer-Consumer Problem
 * @param flags:    A struct containing data associated with each Option arguments
 */
void runSimulation(OPTION_ARGS* flag);

#endif // MIZZO_H