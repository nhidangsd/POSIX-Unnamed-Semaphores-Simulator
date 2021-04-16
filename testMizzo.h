#ifndef TESTMIZZO_H
#define TESTMIZZO_H
#include "mizzo.h"

void testProcessArgs(OPTION_ARGS* flags);
void testInitShareData(SHARE_DATA* sharePtr);
void testInitSemData(SEM_DATA semData);
void testProductionDone(PRODUCER_DATA * ThreadPtr);
void testConsumptionDone(CONSUMER_DATA * ThreadPtr);
void testUpdateIn(PRODUCER_DATA * ThreadPtr, ProductType actualRes);
void testUpdateOut(CONSUMER_DATA* ThreadPtr, ProductType actualRes);
void testInitProducerData(PRODUCER_DATA* threadPtr);
void testInitConsumerData(CONSUMER_DATA* threadPtr);
void printConveyerBelt(int OnBelt[], int in, int out);
#endif // TESTMIZZO_H