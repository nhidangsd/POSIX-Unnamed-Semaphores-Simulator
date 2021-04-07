#include <stdio.h>
#include <assert.h>

#include "testMizzo.h"

void testProcessArgs(OPTION_ARGS flags)
{
    assert(flags.E == 35);
    assert(flags.f == 5);
    assert(flags.e == 15);
    assert(flags.L == 20);
    assert(!(flags.L == 0));
    printf("PASS TEST::testProcessArgs\n");
};