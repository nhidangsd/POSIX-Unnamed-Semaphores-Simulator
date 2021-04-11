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
    printf("PASSED TEST::testProcessArgs\n");
};

void testInitSemData(SEM_DATA semData){
    int val;
    sem_getvalue(&(semData.MutexPtr), &val);
    assert(val == 1);
    sem_getvalue(&(semData.EmptyPtr), &val);
    assert(val == 10);
    sem_getvalue(&(semData.FullPtr), &val);
    assert(val == 0);
    printf("PASSED TEST::testInitSemData\n\n");
};

void testInitThreadData(THREAD_DATA data){
    assert(data.Operation == INCREMENT);
    assert(strcmp(data.Name, "Ethel") == 0);
    assert(data.N == 35);
    assert(data.SemPtr != NULL);
    assert(*(data.ValuePtr) == 0);
    printf("PASSED TEST::testInitThreadData\n\n");
}
