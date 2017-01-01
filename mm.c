/*
 * mm.c
 *
 * - 内核的内存管理机制
 */

#include <kernel.h>
#include <mm.h>
#include <init.h>
#include <string.h>

static pframe_t free_phys_tiny_pages[MAX_FREE_PHYS_TINY_PAGES];
pframe_t *free_phys_tiny_pages_top = free_phys_tiny_pages;
static pframe_t free_phys_pages[MAX_FREE_PHYS_TINY_PAGES];
pframe_t *free_phys_pages_top = free_phys_pages;
static struct chunk_hdr *heap_curr = NULL;

void __mm_init()
{
	asm ("");
	pframe_t page_addr;
	for (page_addr = 0x80020000;
			page_addr < 0x80020000 + MAX_FREE_PHYS_TINY_PAGES * PAGE_SIZE;
			page_addr += PAGE_SIZE) {
		free_phys_page(page_addr);
	}
	for (page_addr = 0x80030000;
			page_addr < 0x80030000 + MAX_FREE_PHYS_TINY_PAGES * PAGE_SIZE / 4;
			page_addr += PAGE_SIZE / 4) {
		free_phys_tiny_page(page_addr);
	}
	mmap(cpu_current_mm(), HEAP_ADDR, alloc_phys_page() | MMU_PAGE);
	heap_curr = (struct chunk_hdr *) HEAP_ADDR;
	heap_curr->next = NULL;
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
	const size_t TEST_SIZE = 10;
	void *p = kmalloc(TEST_SIZE);
	memset(p, 0, TEST_SIZE);
	cpu_switch_mm(cpu_current_mm());
}

void mmap(pgent_t *pd, lpage_t la, pgent_t pg)
{
	pgent_t *pt;
	paddr_t pt_paddr;
	size_t pde_ndx = PDE_INDEX(la);
	size_t pte_ndx = PTE_INDEX(la);
	if (unlikely((pd[pde_ndx] & MMU_TYPE_BITS) != MMU_TABLE)) {
		pt_paddr = alloc_phys_tiny_page();
		pt = (pgent_t *) KPHY2LIN(pt_paddr);
		memset(pt, 0, ENTS_PER_PT * sizeof(pgent_t));
		pd[pde_ndx] = pt_paddr | MMU_SPECIAL | MMU_TABLE;
	} else {
		assert(pd[pde_ndx] & MMU_TYPE_BITS != MMU_TABLE);
		pt = (pgent_t *) (pd[pde_ndx] & PAGE_MASK);
	}
	pt[pte_ndx] = pg;
}

pgent_t mgetmap(pgent_t *pd, lpage_t la)
{
	pgent_t *pt;
	pgent_t pg = 0;
	size_t pde_ndx = PDE_INDEX(la);
	size_t pte_ndx = PTE_INDEX(la);
	if (likely((pd[pde_ndx] & MMU_TYPE_BITS) != MMU_TABLE)) {
		pt = (pgent_t *) pd[pde_ndx];
		pg = pt[pte_ndx];
	}
	return pg;
}

void munmap(pgent_t *pd, lpage_t la)
{
	pgent_t *pt;
	size_t pde_ndx = PDE_INDEX(la);
	size_t pte_ndx = PTE_INDEX(la);
	if (unlikely((pd[pde_ndx] & MMU_TYPE_BITS) != MMU_TABLE))
		return;
	pt = (pgent_t *) (pd[pde_ndx] & PAGE_MASK);
	pt[pte_ndx] = 0;
}

void *kmalloc(size_t size)
{
	size += sizeof(struct chunk_hdr);
	size_t page_count = (size + PAGE_SIZE - 1) >> PTE_SHIFT;
	lpage_t page_laddr = ((lpage_t) heap_curr) + heap_curr->size;
	struct chunk_hdr *new = (struct chunk_hdr *) page_laddr;
	pgent_t *pd = cpu_current_mm();
	while (--page_count) {
		mmap(pd, page_laddr, alloc_phys_page() | MMU_PAGE);
		page_laddr += PAGE_SIZE;
	}
	new = heap_curr->next;
	return (void *) new;
}

void kfree(void *p)
{
	if (unlikely(!p))
		return;
}

pframe_t alloc_phys_tiny_page()
{
	return *--free_phys_tiny_pages_top;
}

void free_phys_tiny_page(pframe_t pa)
{
	*free_phys_tiny_pages_top++ = pa;
}

pframe_t alloc_phys_page()
{
	return *--free_phys_pages_top;
}

void free_phys_page(pframe_t pa)
{
	*free_phys_pages_top++ = pa;
}

