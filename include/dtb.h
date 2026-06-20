#pragma once
#include <types.h>

#define FDT_MAGIC 0xEDFE0DD0
#define FDT_BEGIN_NODE 1
#define FDT_END_NODE 2
#define FDT_PROP 3
#define FDT_NOP 4
#define FDT_END 9

#define DTB_MAX_NODES 16 

struct dt_node {
    char *name;
    struct dt_prop *props;
    struct dt_node *children;
    struct dt_node *next;
};

struct dt_prop {
    char *name;
    char *value;
    size_t len;
    struct dt_prop *next;
};

struct fdt_header {
    uint32_t magic;
    uint32_t totalsize;
    uint32_t off_dt_struct;
    uint32_t off_dt_strings;
    uint32_t off_mem_rsvmap;
    uint32_t version;
    uint32_t last_comp_version;
    uint32_t boot_cpuid_phys;
    uint32_t size_dt_strings;
    uint32_t size_dt_struct;
};

struct dt_node *dt_parse(void *fdt);
void *dt_get_prop(struct dt_node *node, char *name, size_t *len);
uint32_t dt_read_u32(struct dt_node *node, char *name, uint32_t *out);


struct dt_node *dt_node_alloc(char *name);
struct dt_prop *dt_prop_alloc(char *name, void *value, size_t len);
