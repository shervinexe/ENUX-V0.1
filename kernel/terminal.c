#include "terminal.h"
#include "string.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

static uint16_t* const vga_buffer = (uint16_t*)VGA_MEMORY;
static uint8_t term_color = 0x0F;
static size_t term_row = 0;
static size_t term_col = 0;

static void terminal_scroll(void) {
    for (size_t y = 1; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
    for (size_t x = 0; x < VGA_WIDTH; x++)
        vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = (term_color << 8) | ' ';
    term_row = VGA_HEIGHT - 1;
}

void terminal_initialize(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++)
        for (size_t x = 0; x < VGA_WIDTH; x++)
            vga_buffer[y * VGA_WIDTH + x] = (term_color << 8) | ' ';
    term_row = 0;
    term_col = 0;
}

void terminal_putc(char c) {
    if (c == '\n') {
        term_col = 0;
        if (++term_row == VGA_HEIGHT)
            terminal_scroll();
        return;
    }

    vga_buffer[term_row * VGA_WIDTH + term_col] = (term_color << 8) | c;
    if (++term_col == VGA_WIDTH) {
        term_col = 0;
        if (++term_row == VGA_HEIGHT)
            terminal_scroll();
    }
}

void terminal_write(const char* str) {
    for (size_t i = 0; str[i]; i++)
        terminal_putc(str[i]);
}

void terminal_clear(void) {
    terminal_initialize();
}

void terminal_process_command(const char* cmd) {
    if (!cmd || cmd[0] == '\0') return;

    if (strcmp(cmd, "help") == 0) {
        terminal_write("Commands:\n  help\n  clear\n  echo <text>\n  about\n");
    } else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
    } else if (strcmp(cmd, "about") == 0) {
        terminal_write("ENUX is a startup os.\n");
    } else if (strncmp(cmd, "echo ", 5) == 0) {
        terminal_write(cmd + 5);
        terminal_putc('\n');
    } else if (strcmp(cmd, "version") == 0) {
        terminal_write("ENUX v0.1\n");
        terminal_write("All rights reserved by shervin_.exe\n");
    } else {
        terminal_write("Unknown command.\n");
    }
}
