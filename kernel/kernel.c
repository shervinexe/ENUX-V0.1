#include <stdint.h>
#include "terminal.h"
#include "keyboard.h"

static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void kernel_main(void) {
    terminal_initialize();
    terminal_write("ENUX Terminal v0.1\nType 'help' for commands.\n> ");
    keyboard_initialize();

    // Poll keyboard (no interrupts yet)
    for (;;) {
        if (inb(0x64) & 1)
            keyboard_handle();
    }
}
