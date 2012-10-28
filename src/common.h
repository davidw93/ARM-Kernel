#ifndef __COMMON_H
#define __COMMON_H

#define SYSTIMERCLO 0x20003004

#define ONESEC 0x00100000
#define HALFSEC 0x00080000
#define QUARTERSEC 0x00040000

extern void P32(unsigned int, unsigned int);
extern unsigned int G32(unsigned int);

void wait(const unsigned int time, const unsigned int condition);

#endif
