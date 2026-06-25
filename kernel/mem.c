#include <types.h>
#include <mem.h>
#include <dtb.h>
#include <common.h>

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
    while (page && page->size < size) {
        last = page;
        page = page->next;
    }
    if (page == NULL)
        return NULL;

    size_t combined_size = size + sizeof(struct page);

    if (page->size == combined_size) {
        if (last)
            last = page->next;
        else
            free_list = page->next;
    } else if (page->size - combined_size < sizeof(struct page)) {
        if (page->next) {
            struct page *curr = (void *)page + combined_size;

            size_t curr_size = page->size + page->next->size - combined_size;
            struct page *next_page = page->next->next;

            curr->next = next_page;
            curr->size = curr_size;
            curr->status = MEMPAGE_FREE;
        }
        // If page->next is undefined then there's nothing we can do except leak the last few bytes of memory.
    } 

    page->status = MEMPAGE_USED;
    page->size = combined_size;
    page->next = NULL;

    return (void *)page + sizeof(struct page);
}

void kfree(void *ptr) {
    struct page *page_info = (struct page *)(ptr - sizeof(struct page));
    struct page *iter = free_list;

    if (!iter) {
        free_list = page_info;
        return;
    }

    while (iter->next && iter->next < page_info) {
        iter = iter->next;
    }

    page_info->next = iter->next;
    iter->next = page_info;

    merge_freelist();
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


