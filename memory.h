#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"

/*
 * Following global vars are defined in mem.S
 */
extern uint32_t TEXT_START;
extern uint32_t TEXT_END;
extern uint32_t DATA_START;
extern uint32_t DATA_END;
extern uint32_t RODATA_START;
extern uint32_t RODATA_END;
extern uint32_t BSS_START;
extern uint32_t BSS_END;
extern uint32_t HEAP_START;
extern uint32_t HEAP_SIZE;

#define PAGE_SIZE 4096
#define PAGE_ORDER 12

#define PAGE_TAKEN  (uint8_t)(1 << 0)
#define PAGE_LAST   (uint8_t)(1 << 1)
#define PAGE_FIRST  (uint8_t)(1 << 2)

/*
 * Page Descriptor 
 * flags:
 * - bit 0: flag if this page is taken(allocated)
 * - bit 1: flag if this page is the last page of the memory block allocated
 */
struct Page {
	uint8_t flags;
};

void page_init();

/* memory management */
extern void *page_alloc(int npages);
extern void page_free(void *p);
extern void page_test();

#endif /* __MEMORY_H__ */