#include <sys/log.h>
#include <sys/log.h>
#include <sys/mem.h>
#include <sys/cpio.h>
#include <early/dtb.h>

struct kernel_info {
    struct dt_node *head;
    void *mem_start;
    size_t mem_size;

    void *initrd_start;
    size_t initrd_size;
};


uint32_t user_regs[31];

struct kernel_info *discover_arch(void *fdt) {
    struct kernel_info *kernel_info = kemalloc(sizeof(struct kernel_info));

    struct dt_node *head = dt_parse(fdt);
    if (head == NULL) {
        printk("[E;F] Failed to parse device tree!");
        return NULL;
    }

    kernel_info->head = head;


    struct dt_node *memory = dt_find_device(head, "memory");
    if (memory == NULL) {
        printk("[E;F] Failed to find the memory device!");
        return NULL;
    }

    size_t len = 0;

    uint8_t *memory_reg = dt_get_prop(memory, "reg", &len);

    if (!memory_reg || len < sizeof(uintptr_t) + sizeof(size_t)) {
        printk("[E;F] reg property for memory in provided device tree is nonexistent or invalid!");
        return NULL;
    }

    uintptr_t base = btohi(*memory_reg);
    size_t size = btohi(*(memory_reg + sizeof(uintptr_t)));

    kernel_info->mem_start = (void *)base;
    kernel_info->mem_size = size;

    
    uint8_t *initrd_reg = dt_get_prop(head, "initrd", &len);
    if (!initrd_reg || len < sizeof(uintptr_t) + sizeof(size_t)) {
        printk("[E;F] initrd property for head of provided device tree is nonexistent or invalid!");
        return NULL;
    }

    base = btohi(*initrd_reg);
    size = btohi(*(initrd_reg + sizeof(uintptr_t)));

    kernel_info->initrd_start = (void *)base;
    kernel_info->initrd_size = size;


    return kernel_info;
}

void kernel_main(size_t hart, void *fdt) {
    printk("wkern version %s (Compiler %s, Linker %s)\n", VERSION, COMPILER_NAME, LINKER_NAME);
    if (hart != 0)
        return;

    struct kernel_info *kernel_info = discover_arch(fdt);

    if (!kernel_info) {
        return;
    }
}

void kernel_trap() {

    
}
