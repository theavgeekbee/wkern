#include <sys/mem.h>
#include <sys/stddef.h>
#include <early/dtb.h>

static char eheap[EHEAP_SIZE] = {0};
static size_t eheap_off = 0;

static struct free_list *page_list = NULL;
static struct free_list *buddy_head = NULL;
static void *buddy_start;
static uint16_t buddy_bitmap = 0;

static inline size_t round_pow2(size_t n) {
    if (n == 0) return 1;
    
    n--;
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;

    if (sizeof(size_t) >= 8) {
        n |= n >> 32;
    }

    n++;
    
    return n;
}

static inline int cnt_zeros_below(uint16_t mask, int bit) {
    uint16_t lower = mask & ((1 << bit) - 1);
    return lower == 0 ? bit : bit - 1 - (7 - __builtin_clz(lower));
}

static void merge_free_list(struct free_list *head) {
    while (head && head->next) {
        if ((uintptr_t)head + head->size == (uintptr_t)head->next) {
            head->next = head->next->next;
            head->next->prev = head;
            head->next->size = (uintptr_t)head->next - (uintptr_t)head;
        } else {
            head = head->next;
        }
    }
}


static void *buddy_alloc(size_t size) {
    if (!buddy_head) {
        struct free_list *free_page = page_list;
        page_list->next->prev = NULL;

        free_page->next = NULL;
        free_page->prev = NULL;

        buddy_head = free_page;
        buddy_start = free_page;
    }
    size = round_pow2(size);
    
    if (size < 256)
        size = 256;

    struct free_list *first = buddy_head;
    while (first && first->size > size) {
        size_t new_size = first->size / 2;
        struct free_list *next = (struct free_list *)((uintptr_t)first + new_size);

        next->next = first->next;
        next->prev = first;
        next->size = new_size;

        first->next = next;
        first->size = new_size;
    }

    buddy_bitmap |= (1 << (((uintptr_t)first - (uintptr_t)buddy_start)/256));

    if (!first->prev && !first->next) {
        buddy_head = NULL;
        return first;
    }

    if (!first->prev) {
        first->next->prev = NULL;
        return first;
    }

    first->prev->next = first->next;
    first->next->prev = first->prev;

    return first;
}

static void buddy_free(void *ptr) {
    struct free_list *head = buddy_head;
    while (head && (uintptr_t)head < (uintptr_t)ptr) {
        if (head->next == NULL) {
            struct free_list *ins_page = ptr;
            head->next = ins_page;
            ins_page->prev = head;
            ins_page->size = cnt_zeros_below(buddy_bitmap, ((uintptr_t)ptr - (uintptr_t)buddy_head)/256);
            goto merge;
        }
        head = head->next;
    }

    
    struct free_list *ins_page = ptr;
    ins_page->next = head->next;
    ins_page->prev = head;
    ins_page->size = (uintptr_t)ins_page->next - (uintptr_t)ins_page;

    head->next = ins_page;
    ins_page->next->prev = ins_page;

merge:
    merge_free_list(buddy_head);
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


void* kmalloc(size_t size, enum AllocationType type) {
    if (type == MEMPAGE_FREEPAGE) {
        buddy_alloc(size);
    }
    return NULL;
}

void* kzalloc(size_t size, enum AllocationType type) {
    char *ptr = kmalloc(size, type);

    if (!ptr)
        return NULL;

    for (size_t i = 0; i < size; i++) {
        *(ptr + i) = 0;
    }

    return ptr;
}

void kfree(void *ptr) {
    if ((uintptr_t)ptr >= (uintptr_t)buddy_start &&
        (uintptr_t)ptr < (uintptr_t)buddy_start + PAGE_SIZE) {
        buddy_free(ptr);
    }
    (void)ptr;
}


