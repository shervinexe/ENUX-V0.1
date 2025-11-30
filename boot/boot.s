; boot/boot.s — GRUB multiboot header + entry point

section .multiboot
align 4
    dd 0x1BADB002             ; magic number
    dd 0x00                   ; flags
    dd -(0x1BADB002 + 0x00)   ; checksum

section .text
global _start
_start:
    mov esp, stack_top        ; set up stack
    extern kernel_main
    call kernel_main          ; jump to C kernel

.hang:
    hlt
    jmp .hang

section .bss
resb 8192                     ; 8 KB stack
stack_top:
