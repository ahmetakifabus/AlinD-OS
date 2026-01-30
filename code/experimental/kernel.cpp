extern "C" void kernel_main();

static inline unsigned char port_byte_in(unsigned short port) {
    unsigned char result;
    __asm__ volatile("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

static inline void port_byte_out(unsigned short port, unsigned char data) {
    __asm__ volatile("out %%al, %%dx" : : "a" (data), "d" (port));
}

static int cursor_x = 0;
static int cursor_y = 0;
unsigned short* const vga_buffer = (unsigned short*) 0xB8000;

void scroll() {
    if (cursor_y >= 25) {
        for (int i = 0; i < 24 * 80; i++) {
            vga_buffer[i] = vga_buffer[i + 80];
        }
        for (int i = 24 * 80; i < 25 * 80; i++) {
            vga_buffer[i] = (unsigned short) ' ' | (unsigned short) 0x0F00;
        }
        cursor_y = 24;
    }
}

void update_cursor(int x, int y) {
    unsigned short pos = y * 80 + x;
    port_byte_out(0x3D4, 0x0F);
    port_byte_out(0x3D5, (unsigned char)(pos & 0xFF));
    port_byte_out(0x3D4, 0x0E);
    port_byte_out(0x3D5, (unsigned char)((pos >> 8) & 0xFF));
}

void print_char(char c, unsigned char color) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else {
        int index = cursor_y * 80 + cursor_x;
        vga_buffer[index] = (unsigned short) c | (unsigned short) (color << 8);
        cursor_x++;
    }

    if (cursor_x >= 80) {
        cursor_x = 0;
        cursor_y++;
    }
    scroll();
    update_cursor(cursor_x, cursor_y);
}

void print_string(const char* str, unsigned char color) {
    for (int i = 0; str[i] != '\0'; i++) {
        print_char(str[i], color);
    }
}

void clear_screen() {
    for (int i = 0; i < 80 * 25; i++) {
        vga_buffer[i] = (unsigned short) ' ' | (unsigned short) 0x0F00;
    }
    cursor_x = 0;
    cursor_y = 0;
    update_cursor(0, 0);
}

char get_key() {
    unsigned char scancode = 0;
    while (!(port_byte_in(0x64) & 0x01));
    scancode = port_byte_in(0x60);
    
    if (scancode & 0x80) return 0;
    
    static const char kbd_us[128] = {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
        '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' '
    };
    
    return kbd_us[scancode];
}

bool str_compare(const char* s1, const char* s2) {
    int i = 0;
    while (s1[i] == s2[i]) {
        if (s1[i] == '\0') return true;
        i++;
    }
    return false;
}

void shell() {
    char buffer[80];
    int length = 0;

    print_string("\nAlinD OS CLI Active.\n", 0x0B);
    print_string("Click into VirtualBox to type.\n\n", 0x0E);

    while (true) {
        print_string("AlinD@root:~$ ", 0x0A);
        length = 0;
        
        while (true) {
            char c = get_key();
            if (c == 0) continue;
            
            if (c == '\n') {
                buffer[length] = '\0';
                print_char('\n', 0x0F);
                break;
            } else if (c == '\b') {
                if (length > 0) {
                    length--;
                    if (cursor_x > 0) cursor_x--;
                    else if (cursor_y > 0) { cursor_y--; cursor_x = 79; }
                    int index = cursor_y * 80 + cursor_x;
                    vga_buffer[index] = (unsigned short) ' ' | (unsigned short) 0x0F00;
                    update_cursor(cursor_x, cursor_y);
                }
            } else if (length < 78) {
                buffer[length++] = c;
                print_char(c, 0x0F);
            }
        }

        if (str_compare(buffer, "help")) {
            print_string("Available: help, ver, clear, reboot\n", 0x0E);
        } else if (str_compare(buffer, "ver")) {
            print_string("AlinD OS v1.0.1 Stable\n", 0x0E);
        } else if (str_compare(buffer, "clear")) {
            clear_screen();
        } else if (str_compare(buffer, "reboot")) {
            unsigned char temp = port_byte_in(0x64);
            while ((temp & 0x02) != 0) temp = port_byte_in(0x64);
            port_byte_out(0x64, 0xFE);
        } else if (length > 0) {
            print_string("Error: Unknown command.\n", 0x0C);
        }
    }
}

extern "C" void kernel_main() {
    clear_screen();
    shell();
}
