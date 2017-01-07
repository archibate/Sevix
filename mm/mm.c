/*
 * mm.c
 *
 * - 内核的内存管理机制
 */

#include <kernel.h>
#include <mm.h>
#include <init.h>
#include <string.h>

static paddr_t free_phys_tiny_pages[MAX_FREE_PHYS_TINY_PAGES];
paddr_t *free_phys_tiny_pages_top = free_phys_tiny_pages;
static paddr_t free_phys_pages[MAX_FREE_PHYS_TINY_PAGES];
paddr_t *free_phys_pages_top = free_phys_pages;
/*static struct chunk_hdr *heap_curr = NULL;*/

void __mm_init()
{
	paddr_t page_addr;
	for (page_addr = PHYS_PAGES_BASE_PADDR;
			page_addr < PHYS_PAGES_BASE_PADDR + MAX_FREE_PHYS_TINY_PAGES * (PAGE_SIZE / 4);
			page_addr += (PAGE_SIZE / 4)) {
		free_phys_tiny_page(page_addr);
	}
	for (; page_addr < (PHYS_PAGES_BASE_PADDR + MAX_FREE_PHYS_TINY_PAGES * (PAGE_SIZE / 4)) + \
				MAX_FREE_PHYS_PAGES * PAGE_SIZE;
			page_addr += PAGE_SIZE) {
		free_phys_page(page_addr);
	}
	/*heap_curr = (struct chunk_hdr *) HEAP_ADDR;
	heap_curr->next = NULL;*/
	//std::cout << "Hello, C++!" << std::endl;
#if	0
	mmap(cpu_current_mm(), 0xC0010000, 0x80010000 | MMU_PAGE);
	cpu_switch_mm(cpu_current_mm());
	int res = (* (int *) 0xC0010000);
	if (res == (* (int *) 0x80010000))
		uart_puts("SUCCEESS!!\n\r");
	else
		uart_puts("FAILURE!!\n\r");
	if (mgetmap(cpu_current_mm(), 0xC0010000) == 0x80010000 | MMU_PAGE)
		uart_puts("SUCCEESS!!\n\r");
	else
		uart_puts("FAILURE!!\n\r");
	munmap(cpu_current_mm(), 0xC0010000);
	cpu_switch_mm(cpu_current_mm());
	res = (* (int *) 0xC0010000);
	if (res != 0)
		uart_puts("HI\n\r");
#endif
	/*const size_t TEST_COUNT = 2;
	void *p = kualloc(TEST_COUNT);
	memset(p, 0, TEST_COUNT * PAGE_SIZE);*/
	/*const size_t TEST_SIZE = 10000;
	void *p = kmalloc(TEST_SIZE);
	memset(p, 0, TEST_SIZE);*/
	cpu_switch_mm(cpu_current_mm());
}

void mmap(page_t *pd, laddr_t la, page_t pg)
{
	page_t *pt;
	paddr_t pt_paddr;
	size_t pde_ndx = PDE_INDEX(la);
	size_t pte_ndx = PTE_INDEX(la);
	if (unlikely(((pd[pde_ndx] & MMU_TYPE_BITS) != MMU_PGTAB) || (pd[pde_ndx] & MMU_SPECIAL == 0))) {
		pt_paddr = alloc_phys_tiny_page();
		pt = (page_t *) KPHY2LIN(pt_paddr);
		memset(pt, 0, ENTS_PER_PT * sizeof(page_t));
		pd[pde_ndx] = pt_paddr | MMU_SPECIAL | MMU_PGTAB | MMU_MANAGER;
	} else {
		pt_paddr = pd[pde_ndx] & PGTAB_MASK;
		pt = (page_t *) KPHY2LIN(pt_paddr);
	}
	pt[pte_ndx] = pg;
	cpu_switch_mm(cpu_current_mm());
}

page_t mgetmap(page_t *pd, laddr_t la)
{
	page_t *pt;
	paddr_t pt_paddr;
	page_t pg = 0;
	size_t pde_ndx = PDE_INDEX(la);
	size_t pte_ndx = PTE_INDEX(la);
	if (likely((pd[pde_ndx] & MMU_TYPE_BITS) != MMU_PGTAB)) {
		pt_paddr = pd[pde_ndx];
		pt = (page_t *) KPHY2LIN(pt_paddr);
		pg = pt[pte_ndx];
	}
	return pg;
}

void munmap(page_t *pd, laddr_t la)
{
	page_t *pt;
	size_t pde_ndx = PDE_INDEX(la);
	size_t pte_ndx = PTE_INDEX(la);
	if (unlikely((pd[pde_ndx] & MMU_TYPE_BITS) != MMU_PGTAB))
		return;
	pt = (page_t *) (pd[pde_ndx] & PAGE_MASK);
	pt[pte_ndx] = 0;
	cpu_switch_mm(cpu_current_mm());
}

void *kualloc(size_t count)
{
	static union page *umap_curr = (union page *) UMAP_ADDR;
#if	0
	assert(((laddr_t) umap_curr) & ~PAGE_MASK == 0);
#endif
	union page *pages = (union page *) umap_curr;
	page_t *pd = cpu_current_mm();
	while (count--) {
		paddr_t pa = alloc_phys_page();
		mmap(pd, (laddr_t) umap_curr++, pa | MMU_PAGE | MMU_WRITEBACK);
	}
	munmap(pd, (laddr_t) umap_curr++);
	return pages;
}

void kufree(void *pages)
{
#if	0
	assert(((laddr_t) pages) & ~PAGE_MASK == 0);
#endif
	/* TODO */
}

void *kmalloc(size_t size)
{
	return kualloc((size + PAGE_SIZE - 1) >> PAGE_SHIFT);
	/*static laddr_t la = HEAP_ADDR;
	paddr_t pa;
	void *result = (void *) la;
	page_t *pd = cpu_current_mm();
	size_t count = (size + PAGE_SIZE - 1) >> PAGE_SHIFT;
	while (--count) {
		pa = alloc_phys_page();
		mmap(pd, la, pa | MMU_PAGE | MMU_WRITEBACK);
		uart_printf("pa=%p, la=%p\n\r", pa, la);
		la += PAGE_SIZE;
		pa += PAGE_SIZE;
	}*/
	/*size += sizeof(struct chunk_hdr);
	size_t page_count = (size + PAGE_SIZE - 1) >> PTE_SHIFT;
	lpage_t page_laddr = ((((lpage_t) heap_curr) + heap_curr->size) + PAGE_SIZE - 1) & PAGE_MASK;
	struct chunk_hdr *new = (struct chunk_hdr *) page_laddr;
	pgent_t *pd = cpu_current_mm();
	while (page_count--) {
		mmap(pd, page_laddr, alloc_phys_page() | MMU_PAGE);
		page_laddr += PAGE_SIZE;
	}
	new = heap_curr->next;
	return (void *) new + sizeof(struct chunk_hdr);
	return result;*/
}

void kfree(void *p)
{
	if (unlikely(!p))
		return;
	kufree(p);
}

paddr_t alloc_phys_tiny_page()
{
	return *--free_phys_tiny_pages_top;
}

void free_phys_tiny_page(paddr_t pa)
{
	*free_phys_tiny_pages_top++ = pa;
}

paddr_t alloc_phys_page()
{
	return *--free_phys_pages_top;
}

void free_phys_page(paddr_t pa)
{
	*free_phys_pages_top++ = pa;
}

