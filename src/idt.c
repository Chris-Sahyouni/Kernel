#include <system.h>

struct idt_entry {
    unsigned short service_routine_addr_lo;
    unsigned short sel;
    unsigned char always0;
    unsigned char flags;
    unsigned short service_routine_addr_hi;
} __attribute__((packed));

struct idt_pointer {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

// define our actual IDT and IDT pointer
struct idt_entry idt[256];
struct idt_pointer idt_ptr;

extern void idt_load();

// base is the address of the service routine
void idt_set_gate(int index, unsigned long base, unsigned short sel, unsigned char flags) {
    idt[index].service_routine_addr_lo = base & 0xffff;
    idt[index].service_routine_addr_hi = (base >> 16) & 0xffff;
    idt[index].sel = sel;
    idt[index].flags = flags;
    idt[index].always0 = 0;
}

void idt_install() {
    idt_ptr.limit = (sizeof(struct idt_entry) * 256) - 1;
    idt_ptr.base = &idt;
    memset(&idt, 0, sizeof(struct idt_entry) * 256); // zero out initial IDT
    // add ISR's here
    idt_load();
}