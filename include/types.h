#pragma once

#define NULL (void*)0

#define size_t __wk_size_t
#define uint8_t __wk_uint8_t
#define uint16_t __wk_uint16_t
#define uint32_t __wk_uint32_t
#define uint64_t __wk_uint64_t
#define be32_to_cpu(x) __builtin_bswap32(x)

typedef __UINTPTR_TYPE__ __wk_size_t;
typedef __UINT8_TYPE__ __wk_uint8_t;
typedef __UINT16_TYPE__ __wk_uint16_t;
typedef __UINT32_TYPE__ __wk_uint32_t;
typedef __UINT64_TYPE__ __wk_uint64_t;

