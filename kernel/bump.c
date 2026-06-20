#include <bump.h>
#include <common.h>

static char eheap[EHEAP_SIZE] = {0};
static size_t eheap_off = 0;

struct mem_info *km_find_memory(struct dt_node *head) {
    struct dt_node *child = head->children;
    while (child) {
        return km_find_memory(child);
        child = child->next;
    }

    if (strlen(head->name) != 0) {
        struct dt_prop *prop_head = head->props;
        while (prop_head) {
            if (strcmp(prop_head->name, "device_type") == 0
                && strcmp(prop_head->value, "memory") == 0) {
                size_t len = 0;
                uint32_t *reg = (uint32_t *)dt_get_prop(head, "reg", &len);
                if (reg && len >= 8) {
                    uint32_t base = be32_to_cpu(reg[0]);
                    uint32_t size = be32_to_cpu(reg[1]);
                    struct mem_info *ret = kemalloc(sizeof(struct mem_info));
                    ret->start = (void*)base;
                    ret->size = (size_t)size;

                    return ret;
                }
            }
            prop_head = prop_head->next;
        }
    }
    return NULL;
} 


void* kemalloc(size_t size) {
    size = (size + 7) & ~7;

    if (eheap_off + size > EHEAP_SIZE) {
        return NULL;
    }

    void *ptr = &eheap[eheap_off];
    eheap_off += size;
    return ptr;
}

