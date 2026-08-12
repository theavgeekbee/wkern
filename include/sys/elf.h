#pragma once
#include <sys/stddef.h>

#define ELF_MAGIC_LE 0x464C457F
#define ELF_PAD 0

#define EI_CLASS 4
#define EI_DATA 5
#define EI_VERSION 6
#define EI_OSABI 7
#define EI_ABI_VERSION 8

#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define ELFCLASS32 1
#define ELFCLASS64 2

#define EV_CURRENT 1

#ifdef __SYS_LITTLE_ENDIAN__
    #define ELFHOSTDATA ELFDATA2LSB
#else
    #define ELFHOSTDATA ELFDATA2MSB
#endif

#ifdef __SYS_64_BIT__
    #define ELFHOSTCLASS ELFCLASS64
#else
    #define ELFHOSTCLASS ELFCLASS32
#endif

struct elf_header {
    uint8_t e_ident[16];

    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;

    uint32_t e_entry;
    uint32_t e_phoff;
    uint32_t e_shoff;

    uint32_t e_flags;

    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;

    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
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

struct elf_program_header {
    uint32_t p_type;
    uint32_t p_offset;
    uint32_t p_vaddr;
    uint32_t p_paddr;
    uint32_t p_filesz;
    uint32_t p_memsz;
    uint32_t p_flags;
    uint32_t p_align;
};

int elf_chk_header(struct elf_header *header);
struct elf_section_header *elf_get_section(struct elf_header *hdr, void *data, uint32_t index);
char *elf_get_section_name(struct elf_header *hdr, struct elf_section_header *section, void *data);
