#include <types.h>
#include <mem.h>
#include <dtb.h>
#include <common.h>

static struct page *free_list;

void* kmalloc(size_t size) {
    struct page *page = free_list;
    while (page != NULL && page->size < size && page->status != MEMPAGE_FREE) {
        page = page->next;
    }
    if (page == NULL)
        return NULL;

    size_t combined_size = size + sizeof(struct page);
    struct page *next = (struct page *)((void *)page + combined_size);
    next->next = page->next;
    next->size = page->size - combined_size;
    next->status = MEMPAGE_FREE;

    page->status = MEMPAGE_USED;
    next->size = combined_size;
    next->next = NULL;

    return (void *)page + sizeof(struct page);
}



void* memcpy(void *to, const void *from, size_t size) {
    char *dest = (char *)to;
    const char *src = (const char *)from;

    while (size--) {
        *dest++ = *src++;
    }

    return dest;
}

void km_init(struct mem_info *info) {
    struct page *base = (struct page *)info->start;
    base->next = NULL;
    base->size = info->size - sizeof(struct page);
    base->status = MEMPAGE_FREE;

    free_list = base;
}
