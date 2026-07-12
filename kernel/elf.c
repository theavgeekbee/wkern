#include <elf.h>
#include <common.h>

int elf_chk_header(struct elf_header *header) {
    uint32_t magic_chk;
    memcpy(&magic_chk, header->e_ident, sizeof(uint32_t));
    if (magic_chk != ltohi(ELF_MAGIC_LE)) {
        return -1;
    }

    return 0;
}
