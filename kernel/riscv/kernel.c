#include <sys/log.h>
#include <sys/mem.h>
#include <sys/cpio.h>
#include <early/bump.h>

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

    printk("memory blocks initialized\n");

    size_t len = 0;
    uint32_t *initrd = dt_get_prop(head, "initrd", &len);
    if (!initrd || len < 8) {
        printk("failed to find initrd!\n");
        return;
    }
    uint32_t base = btohi(initrd[0]);
    uint32_t size = btohi(initrd[1]);
    
    char *file = cpio_get_file((void*)base, (size_t)size, "/sbin/init");
    (void)file;
}

void kernel_trap() {

    
}
