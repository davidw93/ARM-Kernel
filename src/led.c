#include "led.h"
#include "common.h"

void led_init(void)
{
    unsigned int rand_var;
    rand_var = G32(GPFSEL1);
    rand_var&=~(7<<18);
    rand_var|=1<<18;
    P32(GPFSEL1, rand_var);

}

void blink_active(void)
{
    led_flash_multiple(5, HALFSEC);
    led_flash_multiple(2, QUARTERSEC);
}

void led_flash_multiple(unsigned const int count, unsigned const int time)
{
	int i;

	for (i = 0; i < count; i++) 
	{
		led_flash(time);
	}
}

void led_flash(unsigned const int time)
{
    P32(GPSET, 1<<16);
    wait(time, time);
    P32(GPCLR, 1<<16);
    wait(time, 0);
    P32(GPSET, 1<<16);
}
