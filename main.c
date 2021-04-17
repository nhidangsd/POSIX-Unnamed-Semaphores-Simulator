#include "mizzo.h"

int main(int argc, char* argv[]){

    OPTION_ARGS flags = {0};

    /* Read in arguments from command line and load them to flags */
    processArgs(argc, argv, &flags);

    /* Executes the Producer-Consumer Problem */
    runSimulation(&flags);

    return 0;
}