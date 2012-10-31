TARGET = arm-none-eabi
AS = $(TARGET)-as
CC = $(TARGET)-gcc
LD = $(TARGET)-ld
CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -c
LDFLAGS = -T linker.ld
C_SOURCES = led.c common.c main.c mailbox.c framebuffer.c fizzbuzz.c
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_SOURCES = start.s memfunc.s
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

all: asm kernel

clean:
	rm -f *.o
	rm -f *.img
	rm -f *.elf
asm: $(ASM_OBJECTS)

.s.o:
	$(AS) $< -o $@

kernel: $(C_OBJECTS)
	$(LD) $(ASM_OBJECTS) $(C_OBJECTS) -T loader -o kernel.elf
	$(TARGET)-objcopy kernel.elf -O binary kernel.img

.c.o:
	$(CC) $(CFLAGS) $< -o $@
