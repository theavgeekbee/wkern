#include <cpio.h>
#include <types.h>
#include <common.h>
#include <log.h>

static uint32_t parse_hex(const char *p)
{
    uint32_t value = 0;

    for (int i = 0; i < 8; i++) {
        char c = p[i];
        value <<= 4;

        if (c >= '0' && c <= '9')
            value |= c - '0';
        else if (c >= 'A' && c <= 'F')
            value |= c - 'A' + 10;
        else if (c >= 'a' && c <= 'f')
            value |= c - 'a' + 10;
    }
    return value;
}

static size_t align4(size_t x)
{
    return (x + 3) & ~3;
}

void *cpio_get_file(void *archive, size_t size, const char *filename) {
    const uint8_t *p = (uint8_t *)archive;
    const uint8_t *end = archive + size;

    while (p + CPIO_HEADER_LEN <= end) {
        const struct cpio_newc_hdr *hdr =
            (const struct cpio_newc_hdr *)p;

        if (memcmp(hdr->magic, "070701", 6) &&
            memcmp(hdr->magic, "070702", 6)) {
            printk("Error parsing cpio: bad magic number!\n");
            return NULL;
        }

        uint32_t namesize = parse_hex(hdr->namesize);
        uint32_t filesize = parse_hex(hdr->filesize);

        p += CPIO_HEADER_LEN;

        if (p + namesize > end) {
            printk("Error parsing cpio: truncated filename\n");
            return NULL;
        }

        const char *name = (const char *)p;

        if (strcmp(name, "TRAILER!!!") == 0)
            return 0;

        p += align4(namesize);

        if (p + filesize > end) {
            printk("Error parsing cpio: truncated file\n");
            return NULL;
        }

        if (strcmp(name, filename) == 0) {
            return (void*)p;
        }

        p += align4(filesize);
    }
    return NULL;
}

