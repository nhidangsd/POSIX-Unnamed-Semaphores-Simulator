#include <stdlib.h>     /* atoi */
#include <stdio.h>      /* printf */
#include <unistd.h>     /* getopt */
#include "mizzo.h"

void processArgs(int argc, char* argv[], OPTION_ARGS* flags)
{
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


void runSimulation(OPTION_ARGS flags)
{
    int count = 0;  // num of candies produced
}