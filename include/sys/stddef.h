#pragma once
#define NULL (void*)0

#define uintptr_t __wk_uintptr_t
#define size_t __wk_size_t
#define uint8_t __wk_uint8_t
#define uint16_t __wk_uint16_t
#define uint32_t __wk_uint32_t
#define uint64_t __wk_uint64_t

#define htole

#ifdef __SYS_LITTLE_ENDIAN__
    #define ltohs(x) x
    #define ltohi(x) x
    #define htols(x) x
    #define htoli(x) x

    #define btohs(x) __builtin_bswap16(x)
    #define btohi(x) __builtin_bswap32(x)
    #define htobs(x) __builtin_bswap16(x)
    #define htobi(x) __builtin_bswap32(x)
#else
    #define ltohs(x) __builtin_bswap16(x)
    #define ltohi(x) __builtin_bswap32(x)
    #define htols(x) __builtin_bswap16(x)
    #define htoli(x) __builtin_bswap32(x)

    #define btohs(x) x
    #define btohi(x) x
    #define htobs(x) x
    #define htobi(x) x
#endif

typedef __UINTPTR_TYPE__ __wk_uintptr_t;
typedef __SIZE_TYPE__ __wk_size_t;
typedef __UINT8_TYPE__ __wk_uint8_t;
typedef __UINT16_TYPE__ __wk_uint16_t;
typedef __UINT32_TYPE__ __wk_uint32_t;
typedef __UINT64_TYPE__ __wk_uint64_t;


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
