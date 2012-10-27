TARGET = arm-none-eabi
AS = $(TARGET)-as
CC = $(TARGET)-gcc
LD = $(TARGET)-ld
CFLAGS = -Wall -O2 -nostdlib -nostartfiles -ffreestanding -c
LDFLAGS = -T linker.ld
C_SOURCES = blinking.c
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_SOURCES = loader.s
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

all: asm kernel

clean:
	rm -f *.o

asm: $(ASM_OBJECTS)

.s.o:
	$(AS) $< -o $@

kernel: $(C_OBJECTS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@
