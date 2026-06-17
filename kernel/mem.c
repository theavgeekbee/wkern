#include <types.h>
#include <mem.h>

static char eheap[EHEAP_SIZE] = {0};
static size_t eheap_off = 0;

void* kmalloc(size_t size) {
    return (void*)size;
}

void* kemalloc(size_t size) {
    size = (size + 7) & ~7;

    if (eheap_off + size > EHEAP_SIZE) {
        return NULL;
    }

    void *ptr = &eheap[eheap_off];
    eheap_off += size;
    return ptr;
}

void* memcpy(void *to, const void *from, size_t size) {
    char *dest = (char *)to;
    const char *src = (const char *)from;

    while (size--) {
        *dest++ = *src++;
    }

    return dest;
}
