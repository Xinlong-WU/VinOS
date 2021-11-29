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

static inline int _is_soft_first(struct Page *page)
{
    return page->flags & PAGE_SOFT_FIRST;
}

static inline int _is_malloced(struct Page *page)
{
    return page->flags & PAGE_MALLOC;
}

static inline int _get_page_index_from_memory(void * p)
{
    return ((uint32_t)p - _alloc_start)/ PAGE_SIZE;
}

static inline struct Page * _get_page_table()
{
    return (struct Page *)HEAP_START;
}

static inline int _get_page_index(struct Page * p)
{
    return p - _get_page_table();
}

static inline struct Page * _get_page(void * p)
{
    struct Page *page = _get_page_table();
	int pageIndex = _get_page_index_from_memory(p);
    return page + pageIndex;
}

static inline void * _get_page_memory_byIndex(int pageIndex)
{
    return (void *)(_alloc_start + (PAGE_SIZE * pageIndex));
}

static inline void * _get_page_memory(struct Page * page)
{
    return _get_page_memory_byIndex(_get_page_index(page));
}

static inline struct Page * _get_malloc_table(void * pageMem)
{
    return pageMem + ALLOCABLE_SIZE - 1;
}

static inline int _get_malloc_index(void * p)
{
    struct Page * pageInfo = _get_page(p);
    void * pageMem = _get_page_memory(pageInfo);
    int offset = p - pageMem;
    return offset / MALLOC_SIZE;
}

static inline int _get_memory_usage_of_malloc(struct Page * mallocTable)
{
    int usage = 0;
    uint8_t low = mallocTable[MALLOC_TABLE_SIZE-1].flags;
    uint8_t high = (((mallocTable[MALLOC_TABLE_SIZE-2].flags)>>6)<<6);
    usage = low + (high<<2);
    return usage;
}

static inline void _increase_memory_usage_of_malloc(struct Page * mallocTable)
{
    struct Page * low = mallocTable + MALLOC_TABLE_SIZE-1;
    struct Page * high = mallocTable + MALLOC_TABLE_SIZE-2;
    if(low->flags == 255){
        high->flags += (1<<6);
    }
    low->flags++;
}

static inline void _decrease_memory_usage_of_malloc(struct Page * mallocTable)
{
    struct Page * low = mallocTable + MALLOC_TABLE_SIZE-1;
    struct Page * high = mallocTable + MALLOC_TABLE_SIZE-2;
    if(low->flags == 0){
        low->flags = 255;
        high->flags -= (1<<6);
    }
    else{
        low->flags--;
    }
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

    struct Page *page = _get_page_table();
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
	struct Page *pageTable = _get_page_table();
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

void * _alloc_one_page_by_index(int pageIndex){
    void * mem = NULL;
    if(pageIndex <= _num_pages){
        struct Page * pageTable =  _get_page_table();
        _set_flag(pageTable + pageIndex, PAGE_FIRST);
        _set_flag(pageTable + pageIndex, PAGE_TAKEN);
        _set_flag(pageTable + pageIndex, PAGE_LAST);
        mem = _get_page_memory_byIndex(pageIndex);
    }
    return mem;
}

/*
 * Free the memory block
 * - p: start address of the memory block
 */
void page_free(void *p){
    /*
	 * Assert (TBD) if p is invalid
	 */
	if (!p || (uint32_t)p < _alloc_start ||(uint32_t)p >= _alloc_end) {
        panic("Memory Panic: Con't free this memory");
		return;
	}

    /* get the first page descriptor of this memory block */
    struct Page *page = _get_page(p);
    if(_is_malloced(page)){
        printf("About.\nThis memory is controled by malloc, may yuo want use 'free()'?");
        return;
    }
    /* loop and clear all the page descriptors of the memory block */
    if(!_is_first(page)){
        _set_flag(page-1, PAGE_LAST);
    }
	while (!_is_free(page)) {
        if(_is_malloced(page)){
            free(p);
            _clear(page);
            _set_flag(page, PAGE_MALLOC);
            _set_flag(page, PAGE_FIRST);
            _set_flag(page, PAGE_LAST);
            _set_flag(page, PAGE_TAKEN);
        }
		else if (_is_last(page)) {
			_clear(page);
			break;
		} else {
			_clear(page);
		}
        page++;
        p+=PAGE_SIZE;
	}
}

void printPageInfo(struct Page * page, int pageNumToDisplay){
    int counter = 1;
    for(int i = 0;i < pageNumToDisplay; i++,counter++){
        printf("| %d ", (page+i)->flags);

        // if(counter == 4)
        //     printf("|=");
        if(counter == 8)
            printf("\n"); 
        counter%=8;
    }
    printf("\n");
}

void page_test()
{
    printf("\n\n==============> page_test <==============\n\n");

    printPageInfo(_get_page_table(),16);

	printf("void *p = page_alloc(2);\n");
	void *p = page_alloc(2);
	printf("p = 0x%x\n", p);
	//page_free(p);

    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

	printf("void *p2 = page_alloc(7);\n");
    void *p2 = page_alloc(7);
	printf("p2 = 0x%x\n", p2);
    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    printf("page_free(p2);\n");
	page_free(p2);

    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

	printf("void *p3 = page_alloc(4);\n");
	void *p3 = page_alloc(4);
	printf("p3 = 0x%x\n", p3);

    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    printf("page_free(p+4096);\n");
    page_free(p+4096);

    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    printf("void *p4 = page_alloc(4);\n");
	void *p4 = page_alloc(4);
	printf("p4 = 0x%x\n", p4);

    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    printf("void *p5 = page_alloc(1);\n");
	void *p5 = page_alloc(1);
	printf("p5 = 0x%x\n", p5);

    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    printf("page_free(p);\n");
    page_free(p);

    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    printf("void *p6 = page_alloc(1);\n");
	void *p6 = page_alloc(1);
	printf("p5 = 0x%x\n", p6);

    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    printf("\n\n==============> END page_test <==============\n\n");
}

struct Page * malloc_init(void * page){
    struct Page * byteTable = _get_malloc_table(page);
    for(int i = 0; i < MALLOC_TABLE_SIZE; i++)
        _clear(byteTable+i);
    return byteTable;
}

static inline void * _try_malloc(void * pageMem, size_t size)
{
    struct Page * mallocTable = _get_malloc_table(pageMem);
    int totalBlock = size / MALLOC_SIZE;
    if (size % MALLOC_SIZE)
            totalBlock += 1;
    int found = 0;
    for(int i = 0; i < MALLOC_TABLE_SIZE - totalBlock; i++){
        if(_is_free(mallocTable+i)){
            found = 1;
            /* 
			 * meet a free block, continue to check if following
			 * (totalBlock - 1) pages are also unallocated.
			 */
            for(int j = 1; j < totalBlock; j++){
                if (!_is_free(mallocTable+(i+j))){
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
                _set_flag(mallocTable+i, PAGE_FIRST);
                for (int k = 0; k < totalBlock; k++) {
					_set_flag(mallocTable+i+k, PAGE_TAKEN);
                    _increase_memory_usage_of_malloc(mallocTable);
				}
                _set_flag(mallocTable+i+totalBlock-1, PAGE_LAST);
                return (void *)(pageMem + i * MALLOC_SIZE);
            }
        }
    }
    return NULL;
}

void *malloc(size_t size){
    void * mem = NULL;
    if(size > ALLOCABLE_SIZE){
        // if memory required biger than ALLOCABLE_SIZE of one page, alloc page first
        int pageCounter = (size / PAGE_SIZE) + 1;
        mem = page_alloc(pageCounter);
        // if rest part smaller than ALLOCABLE_SIZE 
        int restMem = size % PAGE_SIZE;
        if(restMem <= ALLOCABLE_SIZE){
            int pageIndex = _get_page_index_from_memory(mem);
            struct Page * lastPage =  _get_page_table() + pageIndex + pageCounter - 1;
            _set_flag(lastPage, PAGE_MALLOC);

            // malloc each byte
            void * lastPageMem = mem + ((pageCounter - 1) * PAGE_SIZE);
            struct Page * mallocTable = malloc_init(lastPageMem);
            int mallocSize = restMem / MALLOC_SIZE;
            if (restMem % MALLOC_SIZE)
                mallocSize += 1;
            for(int i = 0; i < mallocSize; i++){
                if(i == 0)
                    _set_flag(mallocTable, PAGE_SOFT_FIRST);
                _set_flag(mallocTable + i, PAGE_TAKEN);
            }
            _set_flag(mallocTable + mallocSize - 1, PAGE_LAST);
        }
        return mem;
    }else{
        // if memory required smaller than ALLOCABLE_SIZE of one page
        // find first free/malloced page in 100 continuous pages.
        struct Page * pageTable =  _get_page_table();
        int pageCounter = 0;
        int firstFreeIndex = -1;
        int pageIndex = 0;
        for(;pageIndex < _num_pages; pageIndex++,pageCounter++){
            // check whether malloced
            if(_is_malloced(pageTable+pageIndex)){
                // check whether enough
                void * pageMem = _get_page_memory_byIndex(pageIndex);
                mem = _try_malloc(pageMem, size);
                // don't enough go on 
                // enough alloc and return
                if(mem != NULL)
                    return mem;
            }
            if(_is_free(pageTable+pageIndex) && firstFreeIndex < 0){
                firstFreeIndex = pageIndex;
            }
            if(pageCounter > 100){
                // if we have find 100 pages
                // try to alloc first free pages
                if(firstFreeIndex >= 0){
                    // alloc page first
                    mem = _alloc_one_page_by_index(firstFreeIndex);
                    _set_flag(_get_page(mem), PAGE_MALLOC);
                    if(mem!=NULL){
                        malloc_init(mem);
                        mem = _try_malloc(mem, size);
                    }
                    return mem;
                }
                // there is no free page in these 100 pages
                // reset the counter
                pageCounter = 0;
            }
        }
    }
    return mem;
}

void free(void *ptr){
    /*
	 * Assert (TBD) if p is invalid
	 */
	if (!ptr || (uint32_t)ptr < _alloc_start ||(uint32_t)ptr >= _alloc_end) {
        panic("Memory Panic: Con't free this memory");
		return;
	}

    /* get the first page descriptor of this memory block */
    struct Page *page = _get_page(ptr);
    if(_is_malloced(page)){
        struct Page * mallocTable = _get_malloc_table(_get_page_memory(page));
        struct Page * mallocBlock = mallocTable + _get_malloc_index(ptr);
        if(!_is_first(mallocBlock)){
            _set_flag(mallocBlock-1, PAGE_LAST);
        }
        while (!_is_free(mallocBlock)){
            if (_is_last(mallocBlock)) {
                _clear(mallocBlock);
                _decrease_memory_usage_of_malloc(mallocTable);
                break;
            }
            else {
                _clear(mallocBlock);
            }
            _decrease_memory_usage_of_malloc(mallocTable);
            mallocBlock++;
        }
    }
    else{
        page_free(ptr);
    }
    // TBD: free whole page if it is empty
    // use mallocBlock[820] store the use count
}

void molloc_test()
{
    printf("\n\n==============> molloc_test <==============\n\n");
    
    page_init();
    
    printf("most operation are in first Pages\n");

    void * page1Mem = _get_page_memory_byIndex(0);
    struct Page * mallocTable = _get_malloc_table(page1Mem);
    printf("page1 = 0x%x\n", page1Mem);
    printf("Page Table:\n");
    printPageInfo(mallocTable, 8);

    printf("void * page1 = page_alloc(1);\n");
    void * page1 = page_alloc(1);
    printf("page1 = 0x%x\n", page1);
    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    printf("void * page2 = page_alloc(1);\n");
    void * page2 = page_alloc(1);
    printf("page2 = 0x%x\n", page2);
    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    printf("free(page1);\n");
	free(page1);
    printf("Page Table:\n");
    printPageInfo(_get_page_table(),16);

    void * p1 = malloc(sizeof(char)*3276);
    printf("Molloc Table:\n");
    printPageInfo(mallocTable+800, 20);
printf("==> %d\n", _get_memory_usage_of_malloc(mallocTable));
    free(p1);
    printf("Molloc Table:\n");
    printPageInfo(mallocTable, 20);
printf("==> %d\n", _get_memory_usage_of_malloc(mallocTable));

    // printf("void * p1 = malloc(sizeof(char));\n");
    // void * p1 = malloc(sizeof(char));
    // printf("p1 = 0x%x\n", p1);
    // printf("Page Table:\n");
    // printPageInfo(_get_page_table(),16);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);

    // printf("void * p2 = malloc(sizeof(short));\n");
    // void * p2 = malloc(sizeof(short));
    // printf("p2 = 0x%x\n", p2);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);

    // printf("void * p3 = malloc(sizeof(int));\n");
    // void * p3 = malloc(sizeof(int));
    // printf("p3 = 0x%x\n", p3);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);

    // printf("void * p4 = malloc(sizeof(long long int));\n");
    // void * p4 = malloc(sizeof(long long int));
    // printf("p4 = 0x%x\n", p4);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);

    // printf("void * p5 = malloc(sizeof(long double));\n");
    // void * p5 = malloc(sizeof(long double));
    // printf("p5 = 0x%x\n", p5);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);

    // printf("free(p5+4);\n");
    // free(p5+4);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);

    // printf("free(p4+4);\n");
    // free(p4+4);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);

    // printf("void * p6 = malloc(sizeof(int));\n");
    // void * p6 = malloc(sizeof(int));
    // printf("p6 = 0x%x\n", p6);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);

    // printf("void * p7 = malloc(sizeof(int)*819);\n");
    // void * p7 = malloc(sizeof(int)*819);
    // printf("p7 = 0x%x\n", p7);
    // printf("Page Table:\n");
    // printPageInfo(_get_page_table(),16);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);
    
    // printf("void * p8 = malloc(sizeof(int)*4000);\n");
    // void * p8 = malloc(sizeof(int)*4000);
    // printf("p8 = 0x%x\n", p8);
    // printf("Page Table:\n");
    // printPageInfo(_get_page_table(),16);

    // printf("free(p8+4096);\n");
    // page_free(p8+4096);
    // printf("Page Table:\n");
    // printPageInfo(_get_page_table(),16);

    // printf("free(p8);\n");
    // page_free(p8);
    // printf("Page Table:\n");
    // printPageInfo(_get_page_table(),16);

    // printf("void * p9 = malloc(sizeof(int));\n");
    // void * p9 = malloc(sizeof(int));
    // printf("p9 = 0x%x\n", p9);
    // printf("Molloc Table:\n");
    // printPageInfo(mallocTable, 40);


    

    printf("\n\n==============> END molloc_test <==============\n\n"); 
}
