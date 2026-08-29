#pragma once
#include <sys/stddef.h>

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

struct mmio_device_meta {
    const char *name;
    void *start;
    size_t size;
};

void* kmalloc(size_t size);
void* kemalloc(size_t size);
void* kzalloc(size_t size);
void kfree(void* ptr);

void km_init(struct mem_info *info);

