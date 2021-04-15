#include <stdio.h>
#include <assert.h>
#include <string.h> /* strcmp */
#include "testMizzo.h"

void testProcessArgs(OPTION_ARGS flags){
    assert(flags.E == 35);
    assert(flags.f == 5);
    assert(flags.e == 15);
    assert(flags.L == 20);
    assert(!(flags.L == 0));
    printf("PASSED TEST 1 ::testProcessArgs\n\n");
};

void testInitBufferData(BUFFER_DATA* bufferPtr){
    assert(bufferPtr->ConsumerCount == 0);
    assert(bufferPtr->ProducerCount == 0);
    int i;
    for(i=0; i<CONVEYER_BELT_MAX; i++){
        assert(bufferPtr->ConveyerBelt[i] == 0);
    }

    assert(bufferPtr->In == 0);
    assert(bufferPtr->Out == 0);

    for(i=0; i<ProductTypeN; i++){
        assert(bufferPtr->OnBelt[i] == 0);
    }
    for(i=0; i<ConsumerTypeN; i++){
        assert(bufferPtr->Consumed[i] == 0);
    }
    printf("PASSED TEST 2 ::testInitBufferData\n\n");
};

void testInitSemData(SEM_DATA semData){
    int val;
    sem_getvalue(&(semData.MutexPtr), &val);
    assert(val == 1);
    sem_getvalue(&(semData.EmptyPtr), &val);
    assert(val == 10);
    sem_getvalue(&(semData.FullPtr), &val);
    assert(val == 0);
    printf("PASSED TEST 3 ::testInitSemData\n\n");
};

void testInitThreadData(THREAD_DATA data){
    assert(data.Operation == CONSUME);
    assert(strcmp(data.Name, "Lucy") == 0);
    assert(data.N == 20);
    assert(data.Counter == 0);
    assert(data.SemPtr != NULL);
    assert(data.BufferPtr != NULL);
    printf("PASSED TEST 4 ::testInitThreadData\n\n");
}

void testProductionDone(THREAD_DATA * ThreadPtr, int actualResult){
    assert(ThreadPtr->BufferPtr != NULL);
    assert(ThreadPtr->BufferPtr->ProducerCount >=0);

    if(ThreadPtr->BufferPtr->ProducerCount < MAX_NUM_OF_PRODUCT){
        assert(actualResult == 0);
    }else{
        assert(actualResult == 1);
    }
    printf("PASSED TEST 5 ::testProductionDone\n\n");
}

void testConsumptionDone(THREAD_DATA * ThreadPtr, int actualResult){

    assert(ThreadPtr->BufferPtr != NULL);
    assert(ThreadPtr->BufferPtr->ConsumerCount >=0);

    if(ThreadPtr->BufferPtr->ConsumerCount < MAX_NUM_OF_PRODUCT){
        assert(actualResult == 0);
    }else{
        assert(actualResult == 1);
    }
    printf("PASSED TEST 6 ::testConsumptionDone\n\n");
}

void testUpdateIn(THREAD_DATA * ThreadPtr, int actualRes){

    assert(ThreadPtr != NULL);
    assert(ThreadPtr->BufferPtr != NULL);
    assert(actualRes == 95);

    printf("PASSED TEST 7 ::testUpdateIn\n\n");
}

void testUpdateOut(THREAD_DATA * ThreadPtr, int actualRes){

    assert(ThreadPtr != NULL);
    assert(ThreadPtr->BufferPtr != NULL);
    assert(actualRes == 95);
    
    printf("PASSED TEST 8 ::testUpdateOut\n\n");
}


