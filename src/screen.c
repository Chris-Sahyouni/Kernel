#include <system.h>

unsigned short* textmemptr;
int attrib = 0x0f; // bg, fg colors
int cursor_x = 0;
int cursor_y = 0;

void scroll(void) {
    unsigned blank, temp;
    blank = 0x20 | (attrib << 8);

    if (cursor_y >= 25) { // row 25 = final row

        // move current text on screen back by one line
        temp = cursor_y - 25 + 1;
        memcpy(textmemptr, textmemptr + temp * 80, (25 - temp) * 80 * 2);

        // set new line to all blanks
        memsetw(textmemptr + (25 - temp) * 80, blank, 80);
        cursor_y = 25 - 1;
    }
}

void move_cursor(void) {
    unsigned temp;
    // in linear memory, index = (y * width) + x
    temp = cursor_y * 80 + cursor_x;

    // send commmand to VGA controller to update cursor
    // indices 14, 15 = high, low bytes of cursor index
    outportb(0x3d4, 14);
    outportb(0x3d5, temp >> 8);
    outportb(0x3d4, 15);
    outportb(0x3d5, temp);
}

void clear_screen() {
    unsigned blank;

    blank = 0x20 | (attrib << 8);

    // set entire screen to blank
    for (int i = 0; i < 25; i++) {
        memsetw(textmemptr + i * 80, blank, 80);
    }

    // update cursor
    cursor_x = 0;
    cursor_y = 0;
    move_cursor();
}

// write a single character to the screen
void put_char(unsigned char c) {
    unsigned short* where;
    unsigned att = attrib << 8;

    switch (c) {
        case 0x08: // backspace
            if (cursor_x != 0) cursor_x--;
            break;
        case 0x09: // tab
            cursor_x = (cursor_x + 8) & ~(8 - 1); // increment to a point that is divisible by 8
            break;
        case '\r': // carriage return
            cursor_x = 0; // bring cursor back to margin
            break;
        case '\n': // newline
            // bring cursor back to margin and advance to the next line
            cursor_x = 0;
            cursor_y++;
            break;
        default: // printable characters
            where = textmemptr + (cursor_y * 80 + cursor_x); // set the pointer to the correct index
            *where = c | att; // set the memory at that location to the character and color
            cursor_x++; // advance the cursor one position
    }

    if (cursor_x >= 80) { // if cursor reaches edge, add new line
        cursor_x = 0;
        cursor_y++;
    }
    scroll();
    move_cursor();
}

// write a whole string to the screen
void put_str(unsigned char* str) {
    for (int i = 0; i < strlen(str); i++) {
        put_char(str[i]);
    }
}

// set foreground and background colors
void set_colors(unsigned char fg_color, unsigned char bg_color) {
    // top 4 bytes = foreground
    // bottom 4 bytes = background
    attrib = (bg_color << 4) | (fg_color & 0x0f);
}

// sets the text-mode VGA pointer
void init_video() {
    textmemptr = (unsigned short*)0xB8000;
    clear_screen();
}