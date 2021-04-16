#include <stdio.h>
#include <assert.h>
#include <string.h> /* strcmp */
#include "testMizzo.h"

void testProcessArgs(OPTION_ARGS* flags){
    assert(flags->f == 1);
    assert(flags->e == 10);
    assert(flags->L == 50);
    assert(flags->E == 50);
    assert(!(flags->L == 0));
    printf("PASSED TEST 1 ::testProcessArgs\n\n");
};

void testInitShareData(SHARE_DATA* sharePtr){
    assert(sharePtr->ConsumerCount == 0);
    assert(sharePtr->ProducerCount == 0);
    assert(sharePtr->In == 0);
    assert(sharePtr->Out == 0);

    int i;
    for(i=0; i<CONVEYER_BELT_MAX; i++){
        assert(sharePtr->ConveyerBelt[i] == 0);
    }
    for(i=0; i<ProductTypeN; i++){
        assert(sharePtr->OnBelt[i] == 0);
    }
    for(i=0; i<ProductTypeN; i++){
        assert(sharePtr->Produced[i] == 0);
    }
    printf("PASSED TEST 2 ::testInitShareData\n\n");
};

void testInitSemData(SEM_DATA semData){
    int val;
    sem_getvalue(&(semData.Mutex), &val);
    assert(val == 1);
    sem_getvalue(&(semData.Empty), &val);
    assert(val == 10);
    sem_getvalue(&(semData.Full), &val);
    assert(val == 0);
    printf("PASSED TEST 3 ::testInitSemData\n\n");
};

// void testInitThreadData(THREAD_DATA data){
//     assert(data.Operation == CONSUME);
//     assert(data.Name == 0);
//     assert(data.N == 20);
//     assert(data.Counter == 0);
//     assert(data.SemPtr != NULL);
//     assert(data.BufferPtr != NULL);
//     printf("PASSED TEST 4 ::testInitThreadData\n\n");
// }

// void testProductionDone(THREAD_DATA * ThreadPtr, int actualResult){
//     assert(ThreadPtr->BufferPtr != NULL);
//     assert(ThreadPtr->BufferPtr->ProducerCount >=0);

//     if(ThreadPtr->BufferPtr->ProducerCount < MAX_NUM_OF_PRODUCT){
//         assert(actualResult == 0);
//     }else{
//         assert(actualResult == 1);
//     }
//     printf("PASSED TEST 5 ::testProductionDone\n\n");
// }

// void testConsumptionDone(THREAD_DATA * ThreadPtr, int actualResult){

//     assert(ThreadPtr->BufferPtr != NULL);
//     assert(ThreadPtr->BufferPtr->ConsumerCount >=0);

//     if(ThreadPtr->BufferPtr->ConsumerCount < MAX_NUM_OF_PRODUCT){
//         assert(actualResult == 0);
//     }else{
//         assert(actualResult == 1);
//     }
//     printf("PASSED TEST 6 ::testConsumptionDone\n\n");
// }

// void testUpdateIn(THREAD_DATA * ThreadPtr, int actualRes){

//     assert(ThreadPtr != NULL);
//     assert(ThreadPtr->BufferPtr != NULL);
//     assert(actualRes == 95);

//     printf("PASSED TEST 7 ::testUpdateIn\n\n");
// }

// void testUpdateOut(THREAD_DATA * ThreadPtr, int actualRes){

//     assert(ThreadPtr != NULL);
//     assert(ThreadPtr->BufferPtr != NULL);
//     assert(actualRes == 95);
    
//     printf("PASSED TEST 8 ::testUpdateOut\n\n");
// }

void testNewProducerData(PRODUCER_DATA* threadPtr){
    assert(threadPtr->Name == FrogBite);
    assert(threadPtr->N == 1);
    assert(threadPtr->SemPtr != NULL);
    assert(threadPtr->SharePtr != NULL);
    printf("PASSED TEST 9 ::testNewProducerData\n\n");
}

void testNewConsumerData(CONSUMER_DATA* threadPtr){
    assert(threadPtr->Name == Lucy);
    assert(threadPtr->N == 50);
    assert(threadPtr->SemPtr != NULL);
    assert(threadPtr->SharePtr != NULL);

    int i;
    for(i=0; i<ProductTypeN; i++){
        assert(threadPtr->Consumed[i] == 0);
    }
    printf("PASSED TEST 10 ::testNewConsumerData\n\n");
}
