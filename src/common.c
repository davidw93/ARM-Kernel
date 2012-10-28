#include "common.h"

void wait(const unsigned int time, const unsigned int condition)
{
    unsigned int rand_var;

    while (1)
    {
        rand_var=G32(SYSTIMERCLO);
        if ((rand_var&=time)==condition) break;
    }
}
