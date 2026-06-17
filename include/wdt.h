#pragma once
#include <types.h>

#define WDT_MAGIC 0x67674141

struct wdt_device {
    uint32_t type;
    uint32_t driver_id;
    uint32_t start;
    uint32_t size;
};

struct wdt_root {
    uint32_t magic;
    uint32_t count;
    struct wdt_device devices[];
};

