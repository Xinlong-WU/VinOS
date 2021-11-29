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
#define MALLOC_SIZE 4
// ALLOCABLE_SIZE memory can be alloced in each page.
// we use byte table which is 820B long to manage the rest memory(3276B) of this page.
#define ALLOCABLE_SIZE 3276
// the last block(820) is used to count the total memory of this page.
#define MALLOC_TABLE_SIZE 820

#define PAGE_TAKEN  (uint8_t)(1 << 0)
#define PAGE_LAST   (uint8_t)(1 << 1)
#define PAGE_FIRST  (uint8_t)(1 << 2)
#define PAGE_MALLOC  (uint8_t)(1 << 3) // is this page managed by malloc()

#define PAGE_SOFT_LAST  (uint8_t)(1 << 4) // (Page Control Only) means there is still some memory controled by malloc() in next page
#define PAGE_SOFT_FIRST  (uint8_t)(1 << 5) // (Malloc Control Only) means there is still some memory controled by page_alloc() in prev page 
// use 6th & 7th bit to count the memory usage.
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
void *malloc(size_t size);
void free(void *ptr);

#endif /* __MEMORY_H__ */