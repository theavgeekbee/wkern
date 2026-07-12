
#include <dtb.h>
#include <bump.h>
#include <common.h>

struct dt_node *dt_parse(void *fdt) {
    struct fdt_header *hdr = fdt;

    if (hdr->magic != FDT_MAGIC) {
        return NULL;
    }

    uint32_t *p = (uint32_t *)(fdt + btohi(hdr->off_dt_struct));
    char *strings = fdt + btohi(hdr->off_dt_strings);

    struct dt_node *root = NULL;
    struct dt_node *stack[DTB_MAX_NODES];
    int sp = 0;

    while (1) {
        uint32_t token = btohi(*p++);
        switch (token) {
            case FDT_BEGIN_NODE: {
                char *name = (char *)p;
                p += (strlen(name) + 3) / 4;

                struct dt_node *node = dt_node_alloc(name);
                if (sp == 0) {
                    root = node;
                } else {
                    node->next = stack[sp-1]->children;
                    stack[sp-1]->children = node;
                }
                stack[sp++] = node;
                break;
            }
            case FDT_PROP: {
                uint32_t len = btohi(*p++);
                uint32_t nameoff = btohi(*p++);
                void *value = p;
                p += (len + 3) / 4;

                struct dt_prop *prop = dt_prop_alloc(strings + nameoff, value, len);
                prop->next = stack[sp-1]->props;
                stack[sp-1]->props = prop;
                break;
            }
            case FDT_END_NODE: 
                sp--;
                break;
            case FDT_END:
                return root;
        }
    }
}

void *dt_get_prop(struct dt_node *node, char *name, size_t *len)  {
    for (struct dt_prop *p = node->props; p; p = p->next) {
        if (strcmp(p->name, name) == 0) {
            if (len) *len = p->len;
            return p->value;
        }
    }
    return NULL;
}

struct dt_node *dt_node_alloc(char *name) {
    struct dt_node *node = kemalloc(sizeof(struct dt_node));
    node->name = name;
    node->props = NULL;
    node->children = NULL;
    node->next = NULL;
    return node;
}

struct dt_prop *dt_prop_alloc(char *name, void *value, size_t len) {
    struct dt_prop *prop = kemalloc(sizeof(struct dt_prop));
    prop->name = name;
    prop->value = value;
    prop->len = len;
    prop->next = NULL;

    return prop;
}


