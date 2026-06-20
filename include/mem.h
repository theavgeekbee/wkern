#pragma once
#include <types.h>

typedef enum {
    PAGE_FREE,
    PAGE_ALLOCATED
} MemoryPageAvailability;

struct page {
    void* locator;
    MemoryPageAvailability availability;
    struct page* next;
};

struct mem_info {
    void* start;
    size_t size;
};

void* kmalloc(size_t size);
void* memcpy(void* to, const void* from,size_t size);

void* kemalloc(size_t size);

extern struct page root;
