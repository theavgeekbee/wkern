#include <log.h>
#include <types.h>
__attribute__((section(".ringbuf"), aligned(64)))
volatile char log_ring[RINGBUF_SIZE];
static int pos = 0;

void stub(char c) {
    (void)c;
    return;
}

console_callback_t callback = stub;

void put_char(char c) {
    log_ring[pos++] = c;
    if (pos >= RINGBUF_SIZE)
        pos = 0;

    callback(c);
}

void put_string(const char *s) {
    while (*s)
        put_char(*s++);
}

void put_uint(unsigned int value, unsigned int radix) {
    char tmp[16];
    const char digits[] = "0123456789abcdef";
    int i = 0;

    if (value == 0) {
        put_char('0');
        return;
    }

    while (value) {
        tmp[i++] = digits[value % radix];
        value /= radix;
    }

    while (i--)
        put_char(tmp[i]);
}

void put_int(int value) {
    if (value < 0) {
        put_char('-');
        put_uint((unsigned int)(-value), 10);
    } else {
        put_uint((unsigned int)value, 10);
    }
}

void vprintk(const char *fmt, __builtin_va_list args) {
    while (*fmt) {
        if (*fmt != '%') {
            put_char(*fmt++);
            continue;
        }

        fmt++;

        switch (*fmt) {
        case '%':
            put_char('%');
            break;

        case 'c':
            put_char((char)__builtin_va_arg(args, int));
            break;

        case 's':
            put_string(__builtin_va_arg(args, const char *));
            break;

        case 'd':
        case 'i':
            put_int(__builtin_va_arg(args, int));
            break;

        case 'u':
            put_uint(__builtin_va_arg(args, unsigned int), 10);
            break;

        case 'x':
            put_uint(__builtin_va_arg(args, unsigned int), 16);
            break;

        default:
            put_char('%');
            put_char(*fmt);
            break;
        }

        fmt++;
    }
}

void printk(const char *fmt, ...) {
    __builtin_va_list args;
    __builtin_va_start(args, fmt);
    vprintk(fmt, args);
    __builtin_va_end(args);
}
