#include <types.h>
#include <wdt.h>
#define UART_BASE 0x10000000  // Example UART address

static inline void putchar(char c) {
    volatile char *uart = (volatile char *)UART_BASE;
    *uart = c;
}

static void print_string(const char *str) {
    while (*str) {
        putchar(*str++);
    }
}

void kernel_main(void *wdt) {
    struct wdt_root *root = wdt;
    print_string("deserialized all!\n");
    uint32_t count = root->count;
    for (uint32_t i = 0; i < count; i++) {
        print_string("deserializing wdt node!\n");
        print_string("device type: ");
        if (root->devices[i].type == 0) {
            print_string("mem");
        } else {
            print_string("unknown");
        }
        print_string("\n");
    }
    while(1);
}
