#pragma once
#include <types.h>

#define ELF_MAGIC_LE 0x464C457F
#define ELF_PAD 0

#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7
#define EI_ABI_VERSION 8

struct elf_header {
    uint8_t e_ident[16];
    uint8_t e_type;
    uint8_t e_machine;
    uint8_t e_version;
    uint8_t e_entry;
    uint8_t e_phoff;
    uint8_t e_shoff;
    uint8_t e_flags;
    uint8_t e_ehsize;
    uint8_t e_phentsize;
    uint8_t e_phnum;
    uint8_t e_shentsize;
    uint8_t e_shnum;
    uint8_t e_shstrndx;
};

struct elf_section_header {
    uint32_t sh_name;
    uint32_t sh_type;
    uint32_t sh_flags;
    uint32_t sh_addr;
    uint32_t sh_offset;
    uint32_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint32_t sh_addralign;
    uint32_t sh_entsize;
};

int elf_chk_header(struct elf_header *header);
