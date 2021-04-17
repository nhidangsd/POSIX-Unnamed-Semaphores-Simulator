#ifndef MIZZO_H
#define MIZZO_H

#include "structure.h"

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


/* produce(void* VoidPtr)
 * - Executes Producer's task.
 * @param VoidPtr: A pointer pointing to PRODUCER_DATA 
 */
void* produce(void* VoidPtr);


/* consume(void* VoidPtr)
 * - Executes Consumer's task.
 * @param VoidPtr: A pointer pointing to CONSUMER_DATA 
 */
void* consume(void* VoidPtr);


/* updateIn(PRODUCER_DATA * ThreadPtr)
 * - Adds product to the buffer
 * @param ThreadPtr:    A pointer pointing to PRODUCER_DATA 
 */
ProductType updateIn(PRODUCER_DATA * ThreadPtr);


/* updateOut(PRODUCER_DATA * ThreadPtr)
 * - Removes product from the buffer
 * @param ThreadPtr:    A pointer pointing to CONSUMMER_DATA 
 */
ProductType updateOut(CONSUMER_DATA * ThreadPtr);

#endif // MIZZO_H