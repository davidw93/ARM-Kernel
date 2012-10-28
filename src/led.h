#ifndef __LED_H
#define __LED_H

#define GPFSEL1 0x20200004
#define GPSET  0x2020001C
#define GPCLR  0x20200028

void led_init();
void blink_active();
void led_flash_multiple(unsigned const int count, unsigned const int time);
void led_flash(unsigned const int time);

#endif
