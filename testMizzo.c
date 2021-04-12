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
    assert(strcmp(data.Name, "Ethel") == 0);
    assert(data.N == 35);
    assert(data.Counter == 0);
    assert(data.SemPtr != NULL);
    assert(data.BufferPtr != NULL);
    printf("PASSED TEST 4::testInitThreadData\n\n");
}
