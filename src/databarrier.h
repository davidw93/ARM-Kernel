#ifndef __DATABARRIER_H
#define __DATABARRIER_H

#define DATA_MEMBAR() asm volatile ( "mcr p15, #0, %[zero], c7, c10, #5" : : [zero] "r" (0) )
#define DATA_SYNCBAR() asm volatile ( "mcr p15, #0, %[zero], c7, c10, #4" : : [zero] "r" (0) )
#define FLUSH_CACHE() asm volatile ( "mcr p15, #0, %[zero], c7, c14, #0" : : [zero] "r" (0) )

#endif
