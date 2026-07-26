#include <types.h>
#include <dtb.h>
#include <bump.h>
#include <log.h>

uint32_t user_regs[31];

void kernel_main(size_t hart, void *fdt) {
    printk("Doing kernel init with hard ID %d", hart);

    struct dt_node *head = dt_parse(fdt);
    struct mem_info *info = km_find_memory(head);


    if (!info) {
        printk("failed to find memory info!");
        return;
    }

    km_init(info);
}

void kernel_trap() {

    
}
