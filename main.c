#include <stdio.h>
#include <assert.h>
#include "mizzo.h"
#include "testMizzo.h"


int main(int argc, char* argv[]){

    /* argc contains the number of entries in argv: 0 to agrc-1
     * argv[] contains pointers t the strings
     * Note that the program name is argv[0]
     */
    OPTION_ARGS flags = {0};
    processArgs(argc, argv, &flags);
    runSimulation(flags);

    return 0;
}