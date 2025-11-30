# === CONFIG ===
TARGET = myos
ISO = $(TARGET).iso
OBJS = boot/boot.o kernel/kernel.o kernel/terminal.o kernel/keyboard.o kernel/string.o

CC = i686-elf-gcc
ASM = nasm
LD = i686-elf-ld

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra
ASMFLAGS = -f elf32
LDFLAGS = -T linker.ld -nostdlib

# === RULES ===

all: $(ISO)

boot/boot.o: boot/boot.s
	$(ASM) $(ASMFLAGS) $< -o $@

kernel/%.o: kernel/%.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

$(ISO): kernel.bin
	mkdir -p iso/boot/grub
	cp kernel.bin iso/boot/kernel.bin
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo 'menuentry "MyOS" { multiboot /boot/kernel.bin }' >> iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) iso

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf iso $(OBJS) kernel.bin $(ISO)
