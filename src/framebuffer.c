#include "framebuffer.h"
#include "databarrier.h"
#include "mailbox.h"
#include "led.h"
#include "common.h"

#define FAIL_GET_RESOLUTION		1
#define FAIL_GOT_INVALID_RESOLUTION	2
#define FAIL_SETUP_FRAMEBUFFER		3
#define FAIL_INVALID_PITCH_RESPONSE	4
#define FAIL_INVALID_PITCH_DATA		5
#define FAIL_INVALID_TAGS		6
#define FAIL_INVALID_TAG_RESPONSE	7
#define FAIL_INVALID_TAG_DATA		8

#define CHARSIZE_X	6
#define CHARSIZE_Y	10

#define BUFFER_ADDRESS 0X1000

static unsigned int screenbase, screensize;
static unsigned int fb_x, fb_y, pitch;
static unsigned int max_x, max_y;

static void framebuffer_fail(unsigned int num)
{
    while(1)
    {
        led_flash_multiple(num, HALFSEC);
        led_flash_multiple(2, QUARTERSEC);
    }
}

void framebuffer_init()
{
    unsigned int var;
    unsigned int count;
    volatile unsigned int *mailbuffer = (unsigned int*) BUFFER_ADDRESS;

    mailbuffer[0] = 8 * 4;
    mailbuffer[1] = 0;
    mailbuffer[2] = 0x40003;
    mailbuffer[3] = 8;
    mailbuffer[4] = 0;
    mailbuffer[5] = 0;
    mailbuffer[6] = 0;
    mailbuffer[7] = 0;

    mailbox_write(8, (unsigned int)mailbuffer);
    var = mailbox_read(8);

    if(mailbuffer[1] != 0x80000000)
    {
        framebuffer_fail(FAIL_GET_RESOLUTION);
    }

    fb_x = mailbuffer[5];
    fb_y = mailbuffer[6];
    if(fb_x==0 || fb_y==0)
    {
        framebuffer_fail(FAIL_GOT_INVALID_RESOLUTION);
    }

    unsigned int c = 1;
    mailbuffer[c++] = 0;

    mailbuffer[c++] = 0x00048003;
    mailbuffer[c++] = 8;
    mailbuffer[c++] = 8;
    mailbuffer[c++] = fb_x;
    mailbuffer[c++] = fb_y;

    mailbuffer[c++] = 0x00048004;
    mailbuffer[c++] = 8;
    mailbuffer[c++] = 8;
    mailbuffer[c++] = fb_x;
    mailbuffer[c++] = fb_y;

    mailbuffer[c++] = 0x00048005;
    mailbuffer[c++] = 4;
    mailbuffer[c++] = 4;
    mailbuffer[c++] = 16;

    mailbuffer[c++] = 0x00040001;
    mailbuffer[c++] = 8;
    mailbuffer[c++] = 4;
    mailbuffer[c++] = 16;
    mailbuffer[c++] = 0;

    mailbuffer[c++] = 0;
    mailbuffer[0] = c*4;



    mailbox_write(8, (unsigned int)mailbuffer);

    var = mailbox_read(8);
    if(mailbuffer[1] != 0x80000000)
    {
        framebuffer_fail(FAIL_SETUP_FRAMEBUFFER);
    }

    count = 2;
    while((var = mailbuffer[count]))
    {
        if(var == 0x40001)
        {
            break;
        }
        count += 3+(mailbuffer[count+1]>>2);
        if(count>c)
        {
            framebuffer_fail(FAIL_INVALID_TAGS);
        }
    }
    if(mailbuffer[count+2] != 0x80000008)
    {
        framebuffer_fail(FAIL_INVALID_TAG_RESPONSE);
    }

    screenbase = mailbuffer[count+3];
    screensize = mailbuffer[count+4];

    if(screenbase == 0 || screensize == 0)
    {
        framebuffer_fail(FAIL_INVALID_TAG_DATA);
    }

    mailbuffer[0] = 7 * 4;
    mailbuffer[1] = 0;
    mailbuffer[2] = 0x40008;
    mailbuffer[3] = 4;
    mailbuffer[4] = 0;
    mailbuffer[5] = 0;
    mailbuffer[6] = 0;

    mailbox_write(8, (unsigned int)mailbuffer);

    var = mailbox_read(8);

    if(mailbuffer[4] != 0x80000004)
    {
        framebuffer_fail(FAIL_INVALID_PITCH_RESPONSE);
    }
    pitch = mailbuffer[2];
    if(pitch == 0)
    {
        framebuffer_fail(FAIL_INVALID_PITCH_DATA);
    }

    max_x = fb_x / CHARSIZE_X;
    max_y = fb_y / CHARSIZE_Y;
}
