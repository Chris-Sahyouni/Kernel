OUTPUT_FORMAT("binary") /* outputs an elf file */
ENTRY(start)
phys = 0x00100000; /* physical address where the program will be loaded */
SECTIONS /* defines the sections of the binary */
{
    .text phys : AT(phys) { /* specifies the load address, phys in this case */
        code = .; /* assign the current location counter to the "code" symbol */
        *(.text) /* include all .text and .rodata sections from input files */
        *(.rodata)
        . = ALIGN(4096); /* align the next section to this boundary */
    }
    .data : AT(phys +  (data - code)) { /* loaded at an address relative to phys */
        data = .;
        *(.data)
        . = ALIGN(4096);
    }
    .bss : AT(phys + (bss - code)) {
        bss = .;
        *(.bss)
        . = ALIGN(4096);
    }
    end = .;
}

/* The current location counter specifies where in memory the linker script is currently pointing to */