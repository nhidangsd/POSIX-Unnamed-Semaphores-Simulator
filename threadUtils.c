#include "threadUtils.h"
#include "production.h"
#include "io.h"

void initThreadData(THREAD_DATA* threadData, OPERATION operation, 
                    char* name, char* message, 
                    int n, sem_t* mutex, int* val){

    threadData->Operation = operation;
    threadData->Name = name;
    threadData->Message = message;
    threadData->N = n;
    threadData->MutexPtr = mutex;
    threadData->ValuePtr = val;
};


void* operate(void* VoidPtr){
    /* Typecast into a pointer of the expected type. */
    THREAD_DATA	*DataPtr = (THREAD_DATA *) VoidPtr;
    int candy[2] = {0, 0};
    int produce[2] = {1, 0};
    io_add_type(FrogBite, candy, produce);
    io_remove_type(Ethel, FrogBite, candy, produce);
};