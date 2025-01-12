#include <system.h>

unsigned char keymap[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
  '9', '0', '-', '=', '\b',	/* Backspace */
  '\t',			/* Tab */
  'q', 'w', 'e', 'r',	/* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,			/* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
 '\'', '`',   0,		/* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
  'm', ',', '.', '/',   0,				/* Right shift */
  '*',
    0,	/* Alt */
  ' ',	/* Space bar */
    0,	/* Caps lock */
    0,	/* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,	/* < ... F10 */
    0,	/* 69 - Num lock*/
    0,	/* Scroll Lock */
    0,	/* Home key */
    0,	/* Up Arrow */
    0,	/* Page Up */
  '-',
    0,	/* Left Arrow */
    0,
    0,	/* Right Arrow */
  '+',
    0,	/* 79 - End key*/
    0,	/* Down Arrow */
    0,	/* Page Down */
    0,	/* Insert Key */
    0,	/* Delete Key */
    0,   0,   0,
    0,	/* F11 Key */
    0,	/* F12 Key */
    0,	/* All other keys are undefined */
};

// scancode -> ASCII character: ascii_char = keymap[scancode]

// add a global variable to track control, alt, shift, etc

// About the keyboard controller:
// Two channels: one for the keyboard, one for the mouse
// Two registers: Data register at 0x60, Control register at 0x64
//   - Data register is what the keyboard uses to send data to the computer
// Raises IRQ1 when it has data for the computer to read

void keyboard_handler(struct registers* r) {
    unsigned char scancode;
    // read from the data register
    scancode = inportb(0x60);

    // check if the top bit is set
    // if the top bit is set, the user has just released this key
    if (scancode & 0x80) { // KEY RELEASED
        // check here if ctrl, alt, shift, etc have just been released
    }
    else { // KEY PRESSED
        put_char(keymap[scancode]);
    }
}

void keyboard_install() {
    irq_install_handler(1, keyboard_handler);
}