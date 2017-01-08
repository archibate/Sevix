/*
 * mm.c
 *
 * - 内核的内存管理机制
 */

#include <kernel.h>
#include <mm.h>
#include <init.h>
#include <string.h>
#include <asm/serial.h>

#define	MAX_PHYPGMM_PAGES	((PHYPGMM_END_PADDR - PHYPGMM_PADDR) >> PAGE_SHIFT)
static paddr_t free_phys_pages[MAX_PHYPGMM_PAGES];
paddr_t *free_phys_pages_top = free_phys_pages;

void __mm_init()
{
	//paddr_t page_addr;
	/*for (page_addr = PHYS_PAGES_BASE_PADDR;
			page_addr < PHYS_PAGES_BASE_PADDR + MAX_FREE_PHYS_PGTABLES * (PAGE_SIZE / 4);
			page_addr += PGTAB_SIZE) {
		free_phys_pgtable(page_addr);
	}
	for (; page_addr < (PHYS_PAGES_BASE_PADDR + MAX_FREE_PHYS_PGTABLES * (PAGE_SIZE / 4)) + \
				MAX_FREE_PHYS_PAGES * PAGE_SIZE;
			page_addr += PAGE_SIZE) {
		free_phys_page(page_addr);
	}*/
	union page *pages = (union page *) PHYPGMM_PADDR;
	DOTIMES(i, 0, COUNTOF(free_phys_pages))
		free_phys_page((paddr_t) &pages[i]);
#define	alloc_phys_page_table	alloc_phys_page	/* TODO.. */
	IGNOREIT(I think it is pretty ugly :-);
	cpu_switch_mm(cpu_current_mm());
}

#define	PDE_IS_PGTAB(pde) \
	((((pde) & MMU_TYPE_BITS) == MMU_PGTAB) && ((pde) & MMU_SPECIAL == MMU_SPECIAL))

void mmap(page_t *pd, laddr_t la, page_t pg)
{
	page_t *pt;
	paddr_t pt_paddr;
	size_t pde_ndx = PDE_INDEX(la);
	size_t pte_ndx = PTE_INDEX(la);
	page_t pde = pd[pde_ndx];
	if (unlikely(!PDE_IS_PGTAB(pde))) {
		pt_paddr = alloc_phys_page_table();
		pt = (page_t *) KPHY2LIN(pt_paddr);
		SHOW_VALUE(pt, p);
		SHOW_VALUE(pt_paddr, p);
		memset(pt, 0, PGTAB_SIZE);
		pde = pt_paddr | MMU_SPECIAL | MMU_PGTAB | MMU_MANAGER;/**/
		pd[pde_ndx] = pde;
	} else {
		pt_paddr = pde & PGTAB_MASK;
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
	page_t pde = pd[pde_ndx];
	if (likely(PDE_IS_PGTAB(pde))) {
		pt_paddr = pde;
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
	/* TODO: free_phys_page_table */
	pt[pte_ndx] = 0;
	cpu_switch_mm(cpu_current_mm());
}

void *kualloc(size_t count)
{
	static union page *umap_curr = (union page *) UMAP_ADDR;
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
	/* TODO */
}

void *kmalloc(size_t size)
{
	return kualloc((size + PAGE_SIZE - 1) >> PAGE_SHIFT);	/* TODO */
}

void kfree(void *p)
{
	if (unlikely(!p))
		return;
	kufree(p);	/* TODO */
}

paddr_t alloc_phys_page()
{
	return *--free_phys_pages_top;
}

void free_phys_page(paddr_t pa)
{
	*free_phys_pages_top++ = pa;
}

