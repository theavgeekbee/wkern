#pragma once
#include <sys/stddef.h>

#define PAGE_SIZE 4096
#define EHEAP_SIZE (512 * 1024)

struct mem_info {
    void* start;
    size_t size;
};

enum AllocationType {
    MEMPAGE_FREEPAGE, MEMPAGE_ROUNDPAGE, MEMPAGE_VIRTUAL    
};

struct free_list {
    struct free_list *next;
    struct free_list *prev;
    size_t size;
};

struct mmio_device_meta {
    const char *name;
    void *start;
    size_t size;
};

void* kemalloc(size_t size);

void* kmalloc(size_t size, enum AllocationType type);
void* kzalloc(size_t size, enum AllocationType type);
void kfree(void* ptr);

