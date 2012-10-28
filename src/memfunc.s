.globl memcpy
memcpy:
  teq r2,#0
  moveq pc,lr

  stmdb sp!,{lr}
  mov r12,r0
  cmp r2,#0x8
  ble ByteS

  sub r3,r0,r1
  tst r3,#3
  bne ByteS

ByteS:
  ldrb lr,[r1],#1
  subs r2,r2,#1
  strb lr,[r12],#1
  bne ByteS

  ldmia sp!,{pc}

.globl memmove
memmove:
  cmp r0,r1
  bls memcpy

  orr r3,r1,r0
  orr r3,r3,r2
  ands r3,r3,#3

  add r1,r1,r2
  add r2,r2,r0

  beq MOVE1
  b   MOVE2

MOVE1:
  teq r0,r2
  bne MOVE4
  mov pc, lr

MOVE2:
  teq r0,r2
  bne MOVE3
  mov pc,lr

MOVE3:
  ldrb r3,[r1,#-1]!
  strb r3,[r2,#-1]!

MOVE4:
  ldr r3,[r1,#-4]!
  str r3,[r2,#-4]!

.globl memclr
memclr:
  mov r2, r1
  mov r1, #0
