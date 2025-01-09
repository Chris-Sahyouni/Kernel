#include <system.h>

struct gdt_entry {
    unsigned short limit_low;
    unsigned short base_low;
    unsigned char base_middle;
    unsigned char access;
    unsigned char granularity;
    unsigned char base_high;
} __attribute__((packed)); // prevents compiler optimizations

struct gdt_pointer {
    unsigned short max_table_size; // maximum allowed size of the GDT
    unsigned int base; // base address of the GDT
} __attribute__((packed));

// define our gdt table and gdt pointer
struct gdt_entry gdt[3];
struct gdt_pointer gdt_ptr;

// function in start.asm, reloads new segment registers
extern void gdt_flush();

// set up a descriptor in the GDT
void gdt_set_gate(int index, unsigned long base, unsigned long limit, unsigned char access, unsigned char gran) {
    gdt[index].base_low = base & 0xffff;
    gdt[index].base_middle = (base >> 16) & 0xff;
    gdt[index].base_high = (base >> 24) & 0xff;

    gdt[index].limit_low = limit & 0xffff;

    gdt[index].granularity = ((limit >> 16) & 0x0f) | (gran & 0xf0);

    gdt[index].access = access;
}

// function for main to call to set up the GDT
// initializes pointer, three entries, flushes to tell the processor where the new GDT is located
void gdt_install() {
    // initialize GDT pointer
    gdt_ptr.max_table_size = (sizeof(struct gdt_entry) * 3) - 1;
    gdt_ptr.base = &gdt;

    // initialize the table
    // NULL entry
    gdt_set_gate(0, 0, 0, 0, 0);

    // Code segment
    gdt_set_gate(1, 0, 0xffffffff, 0x9a, 0xcf);

    // Data segment
    gdt_set_gate(2, 0, 0xffffffff, 0x92, 0xcf);

    // get rid of the old GDT and replace it with this one
    gdt_flush();
}