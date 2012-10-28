#include "led.h"
#include "framebuffer.h"

int start_kernel(void)
{
    led_init();
    framebuffer_init();
    return 0;
}
