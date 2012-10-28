#include "mailbox.h"
#include "databarrier.h"
#define MAILBOX_FULL 0x80000000
#define MAILBOX_EMPTY 0x40000000

static volatile unsigned int *MAILBOX0READ = (unsigned int *) 0x2000b880;
static volatile unsigned int *MAILBOX0STATUS = (unsigned int *) 0x2000b898;
static volatile unsigned int *MAILBOX0WRITE = (unsigned int *) 0x2000b8a0;

unsigned int mailbox_read(unsigned int channel)
{
    unsigned int count = 0;
    unsigned int data;

    while(1)
    {
        while(*MAILBOX0STATUS & MAILBOX_EMPTY)
        {
            FLUSH_CACHE();
            if(count++>(1<<25))
            {
                return 0xffffffff;
            }
        }

        DATA_MEMBAR();
        data = *MAILBOX0READ;
        DATA_MEMBAR();
        if((data&15)==channel)
        {
            return data;
        }
    }
}

void mailbox_write(unsigned int channel, unsigned int data)
{
    while(*MAILBOX0STATUS & MAILBOX_FULL)
    {
        FLUSH_CACHE();
    }
    DATA_MEMBAR();
    *MAILBOX0WRITE = (data | channel);
}
