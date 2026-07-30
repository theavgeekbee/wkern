#pragma once
#include <types.h>

struct mmio_device_meta {
    const char *name;
    void *start;
    size_t size;
};
