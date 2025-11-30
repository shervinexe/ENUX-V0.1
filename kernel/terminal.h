#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_putc(char c);
void terminal_write(const char* str);
void terminal_clear(void);
void terminal_process_command(const char* cmd);

#endif
