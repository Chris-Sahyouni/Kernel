#include <system.h>

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

// function pointers for custom IRQ handlers
// the pointers will be overwritten in irq_install_handler
void *irq_routines[16] = {
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

// add and IRQ handler
// this second parameter is a function pointer whose functino takes a registers struct as an argument
void irq_install_handler(int irq_number, void (*handler)(struct registers* r)) {
    irq_routines[irq_number] = handler;
}

// remove an IRQ handler
void irq_uninstall_handler(int irq_number) {
    irq_routines[irq_number] = 0;
}

// remap the IRQs from their default locations
// works by sending the relevant commands to the PICs
// master PICs command register is at i/o port 0x20
// slave PICs command register is at i/o port 0xA0
void irq_remap() {
    outportb(0x20, 0x11);
    outportb(0xA0, 0x11);
    outportb(0x21, 0x20);
    outportb(0xA1, 0x28);
    outportb(0x21, 0x04);
    outportb(0xA1, 0x02);
    outportb(0x21, 0x01);
    outportb(0xA1, 0x01);
    outportb(0x21, 0x00);
    outportb(0xA1, 0x00);
}

// remap the PICs and add the new entries to the IDT
void irq_install() {
    irq_remap();

    idt_set_gate(32, (unsigned)irq0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)irq1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)irq2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)irq3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)irq4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)irq5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)irq6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)irq7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned)irq8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)irq9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)irq10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)irq11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)irq12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)irq13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)irq14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)irq15, 0x08, 0x8E);
}

// referenced by every IRQ handler
// the PICs need to be explicitly told when they are done being serviced
// done by sending them an 0x20 to their command registers
void irq_handler(struct registers* r) {
    void (*handler)(struct registers* r); // blank function pointer

    handler = irq_routines[r->interrupt_num - 32];
    if (handler) { // if there is a handler for this interrupt
        handler(r); // then call it
    }

    // we always need to send an end-of-interrupt (EOI) command to the master PIC
    // if the IRQ was from the slave PIC though (IRQs 8-15) then BOTH the master and slave PICs require an EOI command

    if (r->interrupt_num >= 40) { // if IRQ came from the slave PIC
        outportb(0xA0, 0x20); // then send the slave its EOI comamand
    }
    // always send the master PIC its EOI command
    outportb(0x20, 0x20);
}