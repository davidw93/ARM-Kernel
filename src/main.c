#include "led.h"
#include "framebuffer.h"
#include "fizzbuzz.h"

int start_kernel(void)
{
    led_init();
    framebuffer_init();
    fizz_buzz();
    return 0;
}
