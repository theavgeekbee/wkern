#include <types.h>
#include <dtb.h>
#include <bump.h>
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

void kernel_main(size_t hart, void *fdt) {
    (void)hart;

    struct dt_node *head = dt_parse(fdt);
    struct mem_info *info = km_find_memory(head);

    if (info) {
        print_string("found memory!\n");
    } else {
        print_string("could not find memory!\n");
    }

    while(1);
}
