#pragma once
#include <types.h>
#define CPIO_HEADER_LEN 110

struct cpio_newc_hdr {
    char magic[6];
    char ino[8];
    char mode[8];
    char uid[8];
    char gid[8];
    char nlink[8];
    char mtime[8];
    char filesize[8];
    char devmajor[8];
    char devminor[8];
    char rdevmajor[8];
    char rdevminor[8];
    char namesize[8];
    char check[8];
};

void *cpio_get_file(void *archive, size_t size, const char *filename);

