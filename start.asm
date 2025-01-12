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

; Global Descriptor Table
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

; Interrupt Descriptor Table
global idt_load
extern idt_ptr
idt_load:
    lidt [idt_ptr]
    ret

; Interrupt Service Routines
; The first 32 are reserved by Intel for exceptions
global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

; each ISR first pushes an error code (or dummy value of 0 if none is necessary), followed by the interrupt number

isr0: ; Divide by 0 exception
    cli ; disables interrupts
    push byte 0 ; error code
    push byte 0 ; interrupt number
    jmp isr_common_stub

isr1: ; Debug exception
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

isr2: ; Non maskable interrupt exception
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

isr3: ; Breakpoint exception
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

isr4: ; Into detected overflow exception
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

isr5: ; Out of bounds exception
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

isr6: ; Invalid opcode exception
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

isr7: ; No coprocessor exception
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

isr8: ; Double fault exception
    cli
    ; No need to push an error code,
    ; the processor will do this for us when it is required
    push byte 8
    jmp isr_common_stub

isr9: ; Coprocessor segment overrun exception
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

isr10: ; Bad TSS exception
    cli
    push byte 10
    jmp isr_common_stub

isr11: ; Segment not present exception
    cli
    push byte 11
    jmp isr_common_stub

isr12: ; Stack fault exception
    cli
    push byte 12
    jmp isr_common_stub

isr13: ; General protection exception
    cli
    push byte 13
    jmp isr_common_stub

isr14: ; Page fault exception
    cli
    push byte 14
    jmp isr_common_stub

isr15: ; Unknown interrupt exception
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

isr16: ; Coprocessor fault exception
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

isr17: ; Alignment check exception
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

isr18: ; Machine check exception
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

; The rest are reserved

isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub

extern fault_handler

; Saves processor state, sets up kernel mode segments, calls c-level fault handler, restores stack
isr_common_stub:
    ; save processor state
    pusha ; pushes the contents of all general-purpose registers
    push ds
    push es
    push fs
    push gs
    ; the following lines ensure that the kernel's memory segments are used
    mov ax, 0x10 ; load the kernel data segment descriptor
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp ; push us the stack
    push eax
    ; call the c-level fault handler
    mov eax, fault_handler
    call eax ; this way of calling preserves the eip register
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8 ; clean up pushed error code and ISR number
    iret ; pops CS, EIP, EFLAGS, SS, and ESP


; Interrupt Requests
; The Programmable Interrupt Controllers can handle 16 IRQs total
; We need to add these to our newly created IDT
; We will map IRQs 0-15 to entries 32-47 in the IDT
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

irq0:
    cli ; disable interrupts while this one is being serviced
    push byte 0 ; dummy error code
    push byte 32 ; interrupt number
    jmp irq_common_stub

irq1:
    cli
    push byte 0
    push byte 33
    jmp irq_common_stub

irq2:
    cli
    push byte 0
    push byte 34
    jmp irq_common_stub

irq3:
    cli
    push byte 0
    push byte 35
    jmp irq_common_stub

irq4:
    cli
    push byte 0
    push byte 36
    jmp irq_common_stub

irq5:
    cli
    push byte 0
    push byte 37
    jmp irq_common_stub

irq6:
    cli
    push byte 0
    push byte 38
    jmp irq_common_stub

irq7:
    cli
    push byte 0
    push byte 39
    jmp irq_common_stub

irq8:
    cli
    push byte 0
    push byte 40
    jmp irq_common_stub

irq9:
    cli
    push byte 0
    push byte 41
    jmp irq_common_stub

irq10:
    cli
    push byte 0
    push byte 42
    jmp irq_common_stub

irq11:
    cli
    push byte 0
    push byte 43
    jmp irq_common_stub

irq12:
    cli
    push byte 0
    push byte 44
    jmp irq_common_stub

irq13:
    cli
    push byte 0
    push byte 45
    jmp irq_common_stub


irq14:
    cli
    push byte 0
    push byte 46
    jmp irq_common_stub

irq15:
    cli
    push byte 0
    push byte 47
    jmp irq_common_stub

extern irq_handler

; identical to isr_common_stub except that it calls irq_handler instead of fault_handler
irq_common_stub:
    pusha
    push ds
    push es
    push fs
    push gs
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp
    push eax
    mov eax, irq_handler
    call eax
    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8
    iret

; BSS section definition
;   - the BSS section stores uninitialized data,
SECTION .bss
    resb 8192 ; reserve 8 Kb of memory for our stack
_sys_stack: