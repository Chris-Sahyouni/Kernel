; Kernel entry point

[BITS 32] ; specifies we are working with 32 bits
global start ; declares "start" as the entrypoint
start:
    mov esp, _sys_stack ; point the stack pointer to our new stack
    jmp stublet

; must 4 byte align this part
ALIGN 4
mboot: ; used by GRUB to locate the kernel
    ; macros
    MULTIBOOT_PAGE_ALIGN equ 1<<0
    MULTIBOOT_MEMORY_INFO equ 1<<1
    MULTIBOOT_AOUT_KLUDGE equ 1<<16
    MULTIBOOT_HEADER_MAGIC equ 0x1BADB002
    MULTBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE
    MULTIBOOT_CHECKSUM equ -(MULTIBOOT_HEADER_MAGIC + MULTBOOT_HEADER_FLAGS)
    EXTERN code, bss, end

    ; GRUB multiboot header
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM


    dd mboot
    dd code
    dd bss
    dd end
    dd start

stublet:
    extern main
    call main
    jmp $

; declared in gdt.c
global gdt_flush ; allows c code to link this
extern gdt_ptr
gdt_flush:
    lgdt [gdt_ptr]
    mov ax, 0x10 ; offset to data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    jmp 0x08:flush2 ; offset to code segment
flush2:
    ret ; return to c code


; BSS section definition
;   - the BSS section stores uninitialized data,
SECTION .bss
    resb 8192 ; reserve 8 Kb of memory for our stack
_sys_stack: