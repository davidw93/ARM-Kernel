#include "framebuffer.h"
#include "databarrier.h"
#include "led.h"
#include "mailbox.h"
#include "chars.h"
#include "common.h"

#define FAIL_GET_RESOLUTION		1
#define FAIL_GOT_INVALID_RESOLUTION	2
#define FAIL_SETUP_FRAMEBUFFER		3
#define FAIL_INVALID_TAGS		4
#define FAIL_INVALID_TAG_RESPONSE	5
#define FAIL_INVALID_TAG_DATA		6
#define FAIL_INVALID_PITCH_RESPONSE	7
#define FAIL_INVALID_PITCH_DATA		8

#define CHARSIZE_X	6
#define CHARSIZE_Y	10

#define BUFFER_ADDRESS	0x1000

static unsigned int screenbase, screensize;
static unsigned int fb_x, fb_y, pitch;
static unsigned int max_x, max_y;

unsigned int get_screenbase()
{
	return screenbase;
}

unsigned int get_pitch()
{
	return pitch;
}

void framebuffer_fail(unsigned int num)
{
	while(1)
		led_flash_multiple(num, QUARTERSEC);
}

void framebuffer_init(void)
{
	unsigned int var;
	unsigned int count;
	volatile unsigned int *mailbuffer = (unsigned int *) BUFFER_ADDRESS;

	mailbuffer[0] = 8 * 4;		// Total size
	mailbuffer[1] = 0;		// Request
	mailbuffer[2] = 0x40003;	// Display size
	mailbuffer[3] = 8;		// Buffer size
	mailbuffer[4] = 0;		// Request size
	mailbuffer[5] = 0;		// Space for horizontal resolution
	mailbuffer[6] = 0;		// Space for vertical resolution
	mailbuffer[7] = 0;		// End tag

	mailbox_write(8, (unsigned int)mailbuffer);

	var = mailbox_read(8);

	if(mailbuffer[1] != 0x80000000)
		framebuffer_fail(FAIL_GET_RESOLUTION);

	fb_x = mailbuffer[5];
	fb_y = mailbuffer[6];

	if(fb_x==0 || fb_y==0)
		framebuffer_fail(FAIL_GOT_INVALID_RESOLUTION);



	unsigned int c = 1;
	mailbuffer[c++] = 0;		// Request

	mailbuffer[c++] = 0x00048003;	// Tag id (set physical size)
	mailbuffer[c++] = 8;		// Value buffer size (bytes)
	mailbuffer[c++] = 8;		// Req. + value length (bytes)
	mailbuffer[c++] = fb_x;		// Horizontal resolution
	mailbuffer[c++] = fb_y;		// Vertical resolution

	mailbuffer[c++] = 0x00048004;	// Tag id (set virtual size)
	mailbuffer[c++] = 8;		// Value buffer size (bytes)
	mailbuffer[c++] = 8;		// Req. + value length (bytes)
	mailbuffer[c++] = fb_x;		// Horizontal resolution
	mailbuffer[c++] = fb_y;		// Vertical resolution

	mailbuffer[c++] = 0x00048005;	// Tag id (set depth)
	mailbuffer[c++] = 4;		// Value buffer size (bytes)
	mailbuffer[c++] = 4;		// Req. + value length (bytes)
	mailbuffer[c++] = 16;		// 16 bpp

	mailbuffer[c++] = 0x00040001;	// Tag id (allocate framebuffer)
	mailbuffer[c++] = 8;		// Value buffer size (bytes)
	mailbuffer[c++] = 4;		// Req. + value length (bytes)
	mailbuffer[c++] = 16;		// Alignment = 16
	mailbuffer[c++] = 0;		// Space for response

	mailbuffer[c++] = 0;		// Terminating tag

	mailbuffer[0] = c*4;		// Buffer size

	mailbox_write(8, (unsigned int)mailbuffer);

	var = mailbox_read(8);

	if(mailbuffer[1] != 0x80000000)
		framebuffer_fail(FAIL_SETUP_FRAMEBUFFER);

	count=2;
	while((var = mailbuffer[count]))
	{
		if(var == 0x40001)
			break;

		count += 3+(mailbuffer[count+1]>>2);

		if(count>c)
			framebuffer_fail(FAIL_INVALID_TAGS);
	}

	if(mailbuffer[count+2] != 0x80000008)
		framebuffer_fail(FAIL_INVALID_TAG_RESPONSE);

	screenbase = mailbuffer[count+3];
	screensize = mailbuffer[count+4];

	if(screenbase == 0 || screensize == 0)
		framebuffer_fail(FAIL_INVALID_TAG_DATA);

	mailbuffer[0] = 7 * 4;		// Total size
	mailbuffer[1] = 0;		// Request
	mailbuffer[2] = 0x40008;	// Display size
	mailbuffer[3] = 4;		// Buffer size
	mailbuffer[4] = 0;		// Request size
	mailbuffer[5] = 0;		// Space for pitch
	mailbuffer[6] = 0;		// End tag

	mailbox_write(8, (unsigned int)mailbuffer);

	var = mailbox_read(8);

	if(mailbuffer[4] != 0x80000004)
		framebuffer_fail(FAIL_INVALID_PITCH_RESPONSE);

	pitch = mailbuffer[5];
	if(pitch == 0)
		framebuffer_fail(FAIL_INVALID_PITCH_DATA);

	max_x = fb_x / CHARSIZE_X;
	max_y = fb_y / CHARSIZE_Y;
}

static int consx = 0;
static int consy = 0;

static unsigned short int fgcolour = 0xffff;
static unsigned short int bgcolour = 0;

static unsigned int colour_stack[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
static unsigned int colour_sp = 8;

void framebuffer_vert_scroll(void)
{
	unsigned int source;
	register unsigned int rowbytes = CHARSIZE_Y * pitch;

	source = screenbase + rowbytes;
	memmove((void *)screenbase, (void *)source, (max_y-1)*rowbytes);

	memclr((void *)(screenbase + (max_y-1)*rowbytes), rowbytes);
}

void console_write(char *text)
{
	volatile unsigned short int *ptr;

	unsigned int row, addr;
	int col;
	unsigned char ch;

	while((ch = (unsigned char)*text))
	{
		text++;

                switch(ch)
                {
                    case 1: fgcolour = 0b1111100000000000; continue;
                    case 2: fgcolour = 0b0000011111100000; continue;
                    case 3: fgcolour = 0b0000000000011111; continue;
                    case 4: fgcolour = 0b1111111111100000; continue;
                    case 5: fgcolour = 0b1111100000011111; continue;
                    case 6: fgcolour = 0b0000011111111111; continue;
                    case 7: fgcolour = 0b1111111111111111; continue;
                    case 8: fgcolour = 0b0000000000000000; continue;
                    case 9: fgcolour = (fgcolour >> 1) & 0b0111101111101111; continue;
                    case 10: consx=0; consy++; continue;
                    case 11: /* Colour stack push */
                         if(colour_sp)
                             colour_sp--;
                             colour_stack[colour_sp] =
                             fgcolour | (bgcolour<<16);
                             continue;
                    case 12: /* Colour stack pop */
                         fgcolour = colour_stack[colour_sp] & 0xffff;
                         bgcolour = colour_stack[colour_sp] >> 16;
                         if(colour_sp<8)
                             colour_sp++;
                             continue;
                    case 17: bgcolour = 0b1111100000000000; continue;
                    case 18: bgcolour = 0b0000011111100000; continue;
                    case 19: bgcolour = 0b0000000000011111; continue;
                    case 20: bgcolour = 0b1111111111100000; continue;
                    case 21: bgcolour = 0b1111100000011111; continue;
                    case 22: bgcolour = 0b0000011111111111; continue;
                    case 23: bgcolour = 0b1111111111111111; continue;
                    case 24: bgcolour = 0b0000000000000000; continue;
                    case 25: bgcolour = (bgcolour >> 1) & 0b0111101111101111; continue;
                }

		if(ch<32)
		{
			ch=0;
		}
		else
		{
			if(ch>127)
				ch=0;
			else
				ch-=32;
		}

		for(row=0; row<CHARSIZE_Y; row++)
		{
			addr = (row+consy*CHARSIZE_Y)*pitch + consx*CHARSIZE_X*2;

			for(col=(CHARSIZE_X-2); col>=0; col--)
			{
				ptr = (unsigned short int *)(screenbase+addr);

				addr+=2;

                                if(row<(CHARSIZE_Y-1) && (character[ch][row] & (1<<col)))
                                {
                                    *ptr = fgcolour;
                                } else {
                                    *ptr = bgcolour;
                                }
			}

			ptr = (unsigned short int *)(screenbase+addr);
			*ptr = bgcolour;
		}

		if(++consx >= max_x)
		{
			consx = 0;
			if(consy<(max_y-1))
				consy++;
			else
				framebuffer_vert_scroll();
		}
	}
}
