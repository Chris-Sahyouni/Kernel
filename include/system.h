#ifndef __SYSTEM_H
#define __SYSTEM_H

// main.c
extern unsigned char* memcpy(unsigned char* dest, const unsigned char* src, int count);
extern unsigned char* memset(unsigned char* dest, unsigned char val, int count);
extern unsigned short* memsetw(unsigned short* dest, unsigned short val, int count);
extern int strlen(const char* str);
extern unsigned char inportb(unsigned short _port);
extern void outportb(unsigned short _port, unsigned char _data);

// screen.c
extern void clear_screen();
extern void put_char(unsigned char c);
extern void put_str(unsigned char* str);
extern void set_colors(unsigned char fg_color, unsigned char bg_color);
extern void init_video();

// gdt.c
extern void gdt_install();
extern void gdt_set_gate(int index, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran);

// idt.c
extern void idt_install();
extern void idt_set_gate(int index, unsigned long base, unsigned short sel, unsigned char flags);

// isrs.c
extern void isrs_install();

// irq.c
extern void irq_install();
extern void irq_install_handler();
extern void irq_uninstall_handler();

// timer.c
extern void timer_install();

// keyboard.c
extern void keyboard_install();

// defines how our stack looked after an ISR has run
struct registers {
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int interrupt_num, err_code;    /* our 'push byte #' and error codes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */
};

#endif