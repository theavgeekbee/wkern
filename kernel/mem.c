#include <types.h>
#include <mem.h>
#include <dtb.h>
#include <common.h>

void* kmalloc(size_t size) {
    return (void*)size;
}

void* memcpy(void *to, const void *from, size_t size) {
    char *dest = (char *)to;
    const char *src = (const char *)from;

    while (size--) {
        *dest++ = *src++;
    }

    return dest;
}

