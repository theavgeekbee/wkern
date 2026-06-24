#pragma once
#include <types.h>

struct mem_info {
    void* start;
    size_t size;
};

enum PageStatus {
    MEMPAGE_FREE, MEMPAGE_USED
};

struct page {
    struct page* next;
    size_t size;
    enum PageStatus status;
};

void* kmalloc(size_t size);
void* kzmalloc(size_t size);
void kfree(void* ptr);

void* memcpy(void* to, const void* from, size_t size);

void km_init(struct mem_info *info);

