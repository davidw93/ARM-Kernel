extern void P32 ( unsigned int, unsigned int );
extern unsigned int G32 ( unsigned int );
extern void freeze ( unsigned int );

#define GPFSEL1 0x20200004
#define GPSET  0x2020001C
#define GPCLR  0x20200028

int start_kernel( void ) {
    unsigned int rand_var;

    rand_var = G32(GPFSEL1);
    rand_var&=~(7<<18);
    rand_var|=1<<18;
    P32(GPFSEL1, rand_var);

    while( 1 ) {
        P32(GPSET, 1<<16);  /* We want to set it to 16 due to that being the LED for active */
        for(rand_var=0;rand_var<0x100000;rand_var++) freeze(rand_var);
        P32(GPCLR, 1<<16);
        for(rand_var=0;rand_var<0x100000;rand_var++) freeze(rand_var);
    }
    return 0;
}
