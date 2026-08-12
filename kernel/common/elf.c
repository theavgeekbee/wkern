#include <sys/elf.h>

int elf_chk_header(struct elf_header *header) {
    uint32_t magic_chk;
    memcpy(&magic_chk, header->e_ident, sizeof(uint32_t));
    if (magic_chk != ltohi(ELF_MAGIC_LE)) {
        return -1;
    }

    if (header->e_ident[EI_CLASS] != ELFHOSTCLASS) {
        return -1;
    }

    if (header->e_ident[EI_DATA] != ELFHOSTDATA) {
        return -1;
    }

    if (header->e_ident[EI_VERSION] != EV_CURRENT) {
        return -1;
    }

    return 0;
}

struct elf_section_header *elf_get_section(struct elf_header *hdr, void *data, uint32_t index) {
    uint8_t *base = (uint8_t *)data;

    if (index >= hdr->e_shnum) {
        return 0;
    }

    return (struct elf_section_header *)(base
        + hdr->e_shoff
        + index * hdr->e_shentsize);
}

char *elf_get_section_name(struct elf_header *hdr, struct elf_section_header *section, void *data) {
    struct elf_section_header *shstrtab = elf_get_section(hdr, data, hdr->e_shstrndx);
    return (char *)((uintptr_t)section->sh_name + (uintptr_t)shstrtab->sh_offset + (uintptr_t)data);
}
