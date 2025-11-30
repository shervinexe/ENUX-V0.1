#include <stdint.h>
#include "terminal.h"

static inline uint8_t inb(uint16_t port) {
    uint8_t value;
    __asm__ volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
    return value;
}

static const char keymap[128] = {
    0,27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/',0,'*',0,' ',0
};

#define BUFFER_SIZE 128
static char input_buffer[BUFFER_SIZE];
static int input_index = 0;

void keyboard_initialize(void) {
    terminal_write("Keyboard ready.\n> ");
}

void keyboard_handle(void) {
    uint8_t scancode = inb(0x60);
    if (scancode & 0x80) return; // key released

    char c = keymap[scancode];
    if (c == '\b') {
        if (input_index > 0) {
            input_index--;
            terminal_putc('\b');
        }
    } else if (c == '\n') {
        input_buffer[input_index] = '\0';
        terminal_putc('\n');
        terminal_process_command(input_buffer);
        input_index = 0;
        terminal_write("> ");
    } else if (c) {
        if (input_index < BUFFER_SIZE - 1) {
            input_buffer[input_index++] = c;
            terminal_putc(c);
        }
    }
}
