TARGET = arm-none-eabi
AS = $(TARGET)-as
CC = $(TARGET)-gcc
LD = $(TARGET)-ld
CFLAGS = -Wall -nostdlib -nostartfiles -nodefaultlibs -c
LDFLAGS = -T linker.ld
C_SOURCES = blinking.c
C_OBJECTS = $(C_SOURCES:.c=.o)
ASM_SOURCES = loader.s
ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

all: asm kernel

asm: $(ASM_OBJECTS)
kernel: $(C_OBJECTS)

.s.o:
	$(AS) -I $< -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
