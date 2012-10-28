#ifndef __MAILBOX_H
#define __MAILBOX_H

extern unsigned int mailbox_read(unsigned int channel);
extern void mailbox_write(unsigned int channel, unsigned int data);

#endif
