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


void processArgs(int argc, char* argv[], OPTION_ARGS* flags);
void runSimulation(OPTION_ARGS flag);

#endif // MIZZO_H