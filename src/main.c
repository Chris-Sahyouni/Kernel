#include <system.h>

unsigned char* memcpy(unsigned char* dest, const unsigned char* src, int count) {
    // copy count bytes from src to dest
    for (int i = 0; i < count; i++) {
        dest[i] = src[i];
    }
    return dest;
}
unsigned char* memset(unsigned char* dest, unsigned char val, int count) {
    // set count bytes at dest to val
    for (int i = 0; i < count; i++) {
        dest[i] = val;
    }
    return dest;
}
unsigned short* memsetw(unsigned short* dest, unsigned short val, int count) {
    // 16-bit version of memset
    for (int i = 0; i < count; i++) {
        dest[i] = val;
    }
    return dest;
}
int strlen(const char* str) {
    // length of str in bytes
    const char* walk = str;
    while (*walk != 0x00) walk++;
    return walk - str;
}
unsigned char inportb(unsigned short _port) {
    // read from I/O port
    unsigned char rv;
    __asm__ __volatile__("inb %1, %0" : "=a" (rv) : "dN" (_port));
        // asm ("assembly code" : output operands : input operands : clobbered registers (registers that the assembly modifies) )
        // %1, %0 = source operand, dest operand
        // =a (rv), output stored in eax and assigned to rv
        // dN (_port), place _port in dx register or use as immediate
    return rv;
}
void outportb(unsigned short _port, unsigned char _data) {
    // write to I/O port
    __asm__ __volatile__("outb %1, %0" :: "dN" (_port), "a" (_data));
}

void main() {
    init_video();
    put_str("test");
    for (;;);
}