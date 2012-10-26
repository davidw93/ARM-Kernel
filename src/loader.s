.globl _start
_start:
  /* First we set the stack pointer to 0x8000 as this is the value
   * Set by broadcom for where the kernel actually starts
  */
  mov sp,#0x8000
  /* Branch and link (copy the address of the next instruction to LR) */
  /* LR == Link Register (R14) */
  bl start_kernel

/* Once we've finished loading, throw ourselves into an infinite loop */
/* This essentially allows us to stop the processor in a nice way to remove power */
hang: b hang

.globl P32
P32:
  /* Store value of r0 to register r1 */
  str r1,[r0]
  /* Returns from the function */
  bx lr

.globl G32
G32:
  /* We obviously don't want to change the register by getting it */
  /* So load r0 back into r0 */
  ldr r0,[r0]
  /* Returns from the function */
  bx lr

.globl freeze
freeze:
  /* Returns from the function so we can use it to give the processor
   * Random crap to do to slow down the execution of functions
  */
  bx lr
