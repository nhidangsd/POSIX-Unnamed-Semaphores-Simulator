#ifndef TESTMIZZO_H
#define TESTMIZZO_H
#include "mizzo.h"

void testProcessArgs(OPTION_ARGS* flags);
void testInitShareData(SHARE_DATA* sharePtr);
void testInitSemData(SEM_DATA semData);
// void testInitThreadData(THREAD_DATA data);
// void testProductionDone(THREAD_DATA * ThreadPtr, int actualResult);
// void testConsumptionDone(THREAD_DATA * ThreadPtr, int actualResult);
// void testUpdateIn(THREAD_DATA * ThreadPtr, int actualRes);
// void testUpdateOut(THREAD_DATA * ThreadPtr, int actualRes);
void testNewProducerData(PRODUCER_DATA* threadPtr);
void testNewConsumerData(CONSUMER_DATA* threadPtr);
#endif // TESTMIZZO_H