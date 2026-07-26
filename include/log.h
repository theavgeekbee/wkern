#pragma once

#define RINGBUF_SIZE 64 * 1024

void vprintk(const char *message, __builtin_va_list args);
void printk(const char *message, ...);
