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

static inline int memcmp(const void *s1, const void *s2, size_t n) {
    const unsigned char *p1 = (const unsigned char *)s1;
    const unsigned char *p2 = (const unsigned char *)s2;

    for (size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }

    return 0;
}

static inline void *memcpy(void *restrict dest, const void* restrict src, size_t size) {
    unsigned char *d = (unsigned char *)dest;
    unsigned char *s = (unsigned char *)src;
    for (size_t i = 0; i < size; i++) {
        d[i] = s[i];
    }
    return d;
}
