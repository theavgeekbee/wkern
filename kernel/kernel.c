#include <types.h>
#include <dtb.h>
#include <bump.h>
#include <log.h>

#define KERNEL_SIZE 3 * 1024 * 1024

uint32_t user_regs[31];

void kernel_main(size_t hart, void *fdt) {
    printk("Doing kernel init with hard ID %d\n", hart);

    struct dt_node *head = dt_parse(fdt);
    struct mem_info *info = km_find_memory(head);


    if (!info) {
        printk("failed to find memory info!\n");
        return;
    }

    info->start = (void *)((uint8_t *)info->start + KERNEL_SIZE);
    info->size -= KERNEL_SIZE;

    km_init(info);

    printk("kernel init was successful\n");
}

void kernel_trap() {

    
}
