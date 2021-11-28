#include "uart.h"
#include "memory.h"

/*
 * _alloc_start points to the actual start address of heap pool
 * _alloc_end points to the actual end address of heap pool
 * _num_pages holds the actual max number of pages we can allocate.
 */
static uint32_t _alloc_start = 0;
static uint32_t _alloc_end = 0;
static uint32_t _num_pages = 0;

static inline void _clear(struct Page *page)
{
    page->flags = 0;
}

static inline int _is_free(struct Page *page)
{
    return !(page->flags & PAGE_TAKEN);
}

static inline void _set_flag(struct Page *page, uint8_t flags)
{
    page->flags |= flags;
}

static inline int _is_last(struct Page *page)
{
    return page->flags & PAGE_LAST;
}

static inline int _is_first(struct Page *page)
{
    return page->flags & PAGE_FIRST;
}

/*
 * align the address to the border of page(4K)
 */
static inline uint32_t _align_page(uint32_t address)
{
	uint32_t order = (1 << PAGE_ORDER) - 1;
	return (address + order) & (~order);
}

void page_init(){

    /* 
	 * We reserved 8 Page (8 x 4096) to hold the Page structures.
	 * It should be enough to manage at most 128 MB (8 x 4096 x 4096) 
	 */

    _num_pages = (HEAP_SIZE / PAGE_SIZE) - 8;
	printf("HEAP_START = %x, HEAP_SIZE = %x, num of pages = %d\n", HEAP_START, HEAP_SIZE, _num_pages);

    struct Page *page = (struct Page *)HEAP_START;
	for (int i = 0; i < _num_pages; i++) {
		_clear(page);
		page++;	
	}

    _alloc_start = _align_page(HEAP_START + 8 * PAGE_SIZE);
    _alloc_end = _alloc_start + (PAGE_SIZE * _num_pages);

    printf("TEXT:   0x%x -> 0x%x\n", TEXT_START, TEXT_END);
	printf("RODATA: 0x%x -> 0x%x\n", RODATA_START, RODATA_END);
	printf("DATA:   0x%x -> 0x%x\n", DATA_START, DATA_END);
	printf("BSS:    0x%x -> 0x%x\n", BSS_START, BSS_END);
	printf("HEAP:   0x%x -> 0x%x\n", _alloc_start, _alloc_end);

}

/*
 * Allocate a memory block which is composed of contiguous physical pages
 * - npages: the number of PAGE_SIZE pages to allocate
 */
void *page_alloc(int npages){
    if(npages > _num_pages){
        panic("Memory Panic: out of memoery. Huge memory is alloced");
        return NULL;
    }
    
    /* Note we are searching the page descriptor bitmaps. */
	int found = 0;
	struct Page *pageTable = (struct Page *)HEAP_START;
    for(int i = 0; i < (_num_pages - npages);i++){
        if(_is_free(pageTable+i)){
            found = 1;
			/* 
			 * meet a free page, continue to check if following
			 * (npages - 1) pages are also unallocated.
			 */
            for(int j = 1; j < npages; j++){
                if (!_is_free(pageTable+(i+j))){
                    found = 0;
                    break;
                }
            }
            /*
			 * get a memory block which is good enough for us,
			 * take housekeeping, then return the actual start
			 * address of the first page of this memory block
			 */
            if(found){
                _set_flag(pageTable+i, PAGE_FIRST);
                for (int k = 0; k < npages; k++) {
					_set_flag(pageTable+i+k, PAGE_TAKEN);
				}
                _set_flag(pageTable+i+npages-1, PAGE_LAST);
                return (void *)(_alloc_start + i * PAGE_SIZE);
            }
        }
    }
    return NULL;
}

/*
 * Free the memory block
 * - p: start address of the memory block
 */
void page_free(void *p){
    /*
	 * Assert (TBD) if p is invalid
	 */
	if (!p || (uint32_t)p >= _alloc_end) {
        panic("Memory Panic: Con't free this memory");
		return;
	}

    /* get the first page descriptor of this memory block */
    struct Page *page = (struct Page *)HEAP_START;
	int pageIndex = ((uint32_t)p - _alloc_start)/ PAGE_SIZE;
    page += pageIndex;
    /* loop and clear all the page descriptors of the memory block */
    if(!_is_first(page)){
        _set_flag(page-1, PAGE_LAST);
    }
	while (!_is_free(page)) {
		if (_is_last(page)) {
			_clear(page);
			break;
		} else {
			_clear(page);
			page++;;
		}
	}
}

void page_test()
{
	printf("void *p = page_alloc(2);\n");
	void *p = page_alloc(2);
	printf("p = 0x%x\n", p);
	//page_free(p);

	printf("void *p2 = page_alloc(7);\n");
    void *p2 = page_alloc(7);
	printf("p2 = 0x%x\n", p2);
	page_free(p2);

	printf("void *p3 = page_alloc(4);\n");
	void *p3 = page_alloc(4);
	printf("p3 = 0x%x\n", p3);

    printf("page_free(p);\n");
    page_free(p);

    printf("void *p4 = page_alloc(4);\n");
	void *p4 = page_alloc(4);
	printf("p4 = 0x%x\n", p4);
}
