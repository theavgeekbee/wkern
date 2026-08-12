#pragma once
#include <sys/stddef.h>
#include <early/dtb.h>
#define EHEAP_SIZE (512 * 1024)


void* kemalloc(size_t size);
struct mem_info *km_find_memory(struct dt_node *head);

