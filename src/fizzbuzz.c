#include "fizzbuzz.h"
#include "framebuffer.h"
#include "common.h"

void fizz_buzz(void)
{
	unsigned char i;
	char *out_index = "";
	for(i = 1; i < 100; i++)
	{
		if(i % 3 == 0)
		{
			console_write("Fizz");
		}
		if(i % 5 == 0)
		{
			console_write("Buzz");
		}
		if(i % 6 == 0)
		{
			console_write("Bang");
		}
		if(!(i % 3 == 0) && !(i % 5 == 0) && !(i % 6 == 0))
		{
			toChar(i, out_index);
			console_write(out_index);
		}
		console_write("\n");
	}
}
