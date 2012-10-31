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

char* toChar(int input, char *output)
{
	if(input / 10 == 0)
	{
		*output++ = input + '0';
		*output = '\0';
		return output;
	}

	output = toChar(input / 10, output);
	*output++ = input % 10 + '0';
	*output = '\0';
	return output;
}
