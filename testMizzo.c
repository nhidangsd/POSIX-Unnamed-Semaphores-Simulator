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
    // printf("PASSED TEST ::testProcessArgs\n\n");
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
    // printf("PASSED TEST ::testInitShareData\n\n");
};

void testInitSemData(SEM_DATA semData){
    int val;
    sem_getvalue(&(semData.Mutex), &val);
    assert(val == 1);
    sem_getvalue(&(semData.Empty), &val);
    assert(val == 10);
    sem_getvalue(&(semData.Full), &val);
    assert(val == 0);
    // printf("PASSED TEST ::testInitSemData\n\n");
};


void testProductionDone(PRODUCER_DATA * ThreadPtr){
    assert(ThreadPtr->SharePtr != NULL);
    assert(ThreadPtr->SharePtr->ProducerCount >=0);
    assert(ThreadPtr->SharePtr->ProducerCount <= MAX_NUM_OF_PRODUCT);
    // printf("PASSED TEST ::testProductionDone\n\n");
}

void testConsumptionDone(CONSUMER_DATA * ThreadPtr){

    assert(ThreadPtr->SharePtr != NULL);
    assert(ThreadPtr->SharePtr->ConsumerCount >=0);

    assert(ThreadPtr->SharePtr->ConsumerCount <= MAX_NUM_OF_PRODUCT);
    // printf("PASSED TEST ::testConsumptionDone\n\n");
}

void testUpdateIn(PRODUCER_DATA * ThreadPtr, ProductType actualRes){

    assert(ThreadPtr != NULL);
    assert(ThreadPtr->SharePtr != NULL);
    assert(actualRes == FrogBite || actualRes == Escargot);

    // printf("PASSED TEST ::testUpdateIn\n\n");
}

void testUpdateOut(CONSUMER_DATA* ThreadPtr, ProductType actualRes){

    assert(ThreadPtr != NULL);
    assert(ThreadPtr->SharePtr != NULL);
    assert(actualRes == FrogBite || actualRes == Escargot);
    // printf("PASSED TEST ::testUpdateOut\n\n");
}

void testInitProducerData(PRODUCER_DATA* threadPtr){
    assert(threadPtr->Name == FrogBite);
    assert(threadPtr->N == 1);
    assert(threadPtr->SemPtr != NULL);
    assert(threadPtr->SharePtr != NULL);
    // printf("PASSED TEST ::testNewProducerData\n\n");
}

void testInitConsumerData(CONSUMER_DATA* threadPtr){
    assert(threadPtr->Name == Lucy);
    assert(threadPtr->N == 50);
    assert(threadPtr->SemPtr != NULL);
    assert(threadPtr->SharePtr != NULL);

    int i;
    for(i=0; i<ProductTypeN; i++){
        assert(threadPtr->Consumed[i] == 0);
    }
    // printf("PASSED TEST ::testNewConsumerData\n\n");
}

void printConveyerBelt(int ConveyerBelt[], int in, int out){
  int i;
  printf("ConveyerBelt = [ ");
  for(i=0; i<CONVEYER_BELT_MAX; i++){
    printf("%d, ", ConveyerBelt[i]);
  }
  printf(" ]\t In=%d\t Out=%d\n\n", in, out);
}