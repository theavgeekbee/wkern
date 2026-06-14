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

void kernel_main(void) {
    print_string("Hello from RISC-V kernel!\n");
    
    while (1) {
        // Wait
    }
}
