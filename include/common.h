#pragma once
#include <types.h>

static inline size_t strlen(const char *s) {
    size_t len = 0;
    while (*s++) {
        ++len;
    }
    return len;
}
static inline int strcmp(const char *str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
    }
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}

static inline void *memcpy(void *restrict dest, const void* restrict src, size_t size) {
    unsigned char *d = (unsigned char *)dest;
    unsigned char *s = (unsigned char *)src;
    for (size_t i = 0; i < size; i++) {
        d[i] = s[i];
    }
    return d;
}
