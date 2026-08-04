#include <log.h>
#include <mmio.h>

#ifndef __KERNEL_MODULE__
#define __KERNEL_MODULE__
#endif

struct console_device_meta {
    void *register_controller;
    void *data_start;
    void *data_end;
};

const char compatible[] __attribute__((section(".compatible"))) = "unix,soc-tty";
size_t pos = 0;
struct console_device_meta dev_meta = { NULL, NULL, NULL };

__attribute__((visibility("hidden"))) void console_put_char(char c) {
    ((char *)dev_meta.data_start)[pos++] = c;

    if (c == '\n') {
        *((uint8_t *)dev_meta.register_controller) = 1;
        pos = 0;
    }
    return;
}


int __attribute__((section(".module_init"))) module_init(struct mmio_device_meta *meta) {
    register_console_callback(console_put_char);

    dev_meta.register_controller = meta->start;
    dev_meta.data_start = (void *)((uint8_t *)meta->start + 1);
    dev_meta.data_end = (void *)((uint8_t *)meta->start + meta->size);

    printk("Registered console device successfully");
    return 0;
}

void __attribute__((section(".module_exit"))) module_exit(void) {
    printk("Unregistered console device (nothing to do)");
}
