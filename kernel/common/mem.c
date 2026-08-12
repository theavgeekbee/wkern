#include <sys/mem.h>
#include <early/dtb.h>

static struct page *free_list;

static void merge_freelist() {
    struct page *list = free_list;

    while (list && list->next) {
        if ((void*)list + list->size == list->next) {
            list->size += list->next->size;
            list->next = list->next->next;
            continue;
        }
        list = list->next;
    }
}

void* kmalloc(size_t size) {
    struct page *page = free_list;
    struct page *last = NULL;

    size_t combined_size = size + sizeof(struct page);

    while (page && page->size < combined_size) {
        last = page;
        page = page->next;
    }

    if (page == NULL)
        return NULL;


    if (page->size - combined_size < sizeof(struct page)) {
        // Return the entire page
        if (last)
            last->next = page->next;
        else
            free_list = page->next;

    } else {
        // break page and create a new one
        struct page *broken = (void *)page + combined_size;
        broken->size = page->size - combined_size;
        broken->next = page->next;
        broken->status = MEMPAGE_FREE;

        page->size = combined_size;
        page->next = broken;

        if (last)
            last->next = broken;
        else
            free_list = broken;
    } 

    page->status = MEMPAGE_USED;

    return (void *)page + sizeof(struct page);
}

void* kzalloc(size_t size) {
    char *ptr = kmalloc(size);

    if (!ptr)
        return NULL;

    for (size_t i = 0; i < size; i++) {
        *(ptr + i) = 0;
    }

    return ptr;
}

void kfree(void *ptr) {
    if (!ptr)
        return;

    struct page *page_info = (struct page *)(ptr - sizeof(struct page));
    struct page *iter = free_list;

    page_info->status = MEMPAGE_FREE;

    if (!iter) {
        free_list = page_info;
        page_info->next = NULL;
        goto merge;
    }
    if (page_info < free_list) {
        page_info->next = iter;
        free_list = page_info;
        goto merge;
    }

    while (iter->next && iter->next < page_info) {
        iter = iter->next;
    }

    page_info->next = iter->next;
    iter->next = page_info;

    merge:
    merge_freelist();
}


void km_init(struct mem_info *info) {
    struct page *base = (struct page *)info->start;
    base->next = NULL;
    base->size = info->size - sizeof(struct page);
    base->status = MEMPAGE_FREE;

    free_list = base;
}


