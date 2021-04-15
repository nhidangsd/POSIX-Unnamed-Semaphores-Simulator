#ifndef TESTMIZZO_H
#define TESTMIZZO_H
#include "mizzo.h"

void testProcessArgs(OPTION_ARGS flags);
void testInitBufferData(BUFFER_DATA* bufferPtr);
void testInitSemData(SEM_DATA semData);
void testInitThreadData(THREAD_DATA data);
void testProductionDone(THREAD_DATA * ThreadPtr, int actualResult);
void testConsumptionDone(THREAD_DATA * ThreadPtr, int actualResult);
void testUpdateIn(THREAD_DATA * ThreadPtr, int actualRes);
void testUpdateOut(THREAD_DATA * ThreadPtr, int actualRes);
#endif // TESTMIZZO_H