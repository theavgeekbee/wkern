#include <types.h>
#include <dtb.h>
#include <bump.h>

void kernel_main(size_t hart, void *fdt) {
    (void)hart;

    struct dt_node *head = dt_parse(fdt);
    struct mem_info *info = km_find_memory(head);

    if (!info) {
        while (1);
    }

    km_init(info);


    while(1);
}
