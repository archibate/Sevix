#ifndef	_MM_H_INCLUDED
#define	_MM_H_INCLUDED

#define	PTE_SHIFT	12
#define	PTE_WIDE_SHIFT	8
#define	PDE_SHIFT	20
#define	PDE_WIDE_SHIFT	12
#define	PAGE_SHIFT	PTE_SHIFT
#define	PGTAB_SHIFT	10
#define	MMU_SPECIAL	(1 << 4)
#define	MMU_TYPE_BITS	(3)
#define	MMU_SECTION	(2)
#define	MMU_PGTAB	(1)
#define	MMU_PAGE	(2)
#define	MMU_DOMAIN_BITS	(16 << 5)
#define	MMU_MANAGER	(2 << 5)
#define	MMU_CLIENT	(1 << 5)
#define	MMU_CACHEABLE	(1 << 2)
#define	MMU_BUFFERABLE	(1 << 3)
#define	MMU_WRITEBACK	(MMU_CACHEABLE | MMU_BUFFERABLE)
#define	PAGE_SIZE	(__R_T_IMM(1) << PTE_SHIFT)
#define	PAGE_MASK	(__R_T_IMM(~0) << PAGE_SHIFT)
#define	PGTAB_MASK	(__R_T_IMM(~0) << PGTAB_SHIFT)
#define	ENTS_PER_PT	(__R_T_IMM(1) << PTE_WIDE_SHIFT)
#define	ENTS_PER_PD	(__R_T_IMM(1) << PDE_WIDE_SHIFT)
#define	PTE_INDEX(la)	__PXE_INDEX(PTE, la)
#define	PDE_INDEX(la)	__PXE_INDEX(PDE, la)
#define	MAX_FREE_PHYS_TINY_PAGES	256
#define	MAX_FREE_PHYS_PAGES		256
#define	PHYS_PAGES_BASE_PADDR	0x80020000
#define	HEAP_ADDR		0xE0000000
#define	UMAP_ADDR		0xD0000000
#define	UMAP_END_ADDR		0xD8000000
#define	KBASE			0xC0000000
#define	KBASE_PADDR		0x80000000
#define	KERNEL_LOADED_PADDR	0x80010000
#define	KBASE_OFFSET		(KBASE - KBASE_PADDR)
#define	KPHY2LIN(phy)	(__CTYPE_CVNT(laddr_t) ((__CTYPE_CVNT(paddr_t) (phy)) + KBASE_OFFSET))
#define	KLIN2PHY(lin)	(__CTYPE_CVNT(laddr_t) ((__CTYPE_CVNT(paddr_t) (lin)) - KBASE_OFFSET))
#if	!defined(_PEDANTIC) && defined(__CLANG__)
#define	__PXE_INDEX(PXE, la) ({ \
		laddr_t __la = (la); \
		__la >>= PXE##_SHIFT; \
		__la &= __R_T_IMM(~0) >> (_BITS - PXE##_WIDE_SHIFT); \
	})
#else
#define	__PXE_INDEX(PXE, la) (((la) >> PXE##_SHIFT) & \
		(__R_T_IMM(~0) >> (_BITS - PXE##_WIDE_SHIFT)))
#endif	/* !defined(_PEDANTIC) && defined(__CLANG__) */

#ifdef	__CLANG__

#ifdef	_PEDANTIC
#define	cpu_switch_mm(pd) \
	(asm ("mcr p15, 0, %0, c2, c0, 0" :: "r" (pd)))
#define	cpu_current_mm(pd) \
	((page_t *) asm ("mcr p15, 0, r0, c2, c0, 0"))
#else	/* _PEDANTIC */
#define	cpu_switch_mm(pd) ({ \
		page_t *__pd = (pd); \
		asm ("mcr p15, 0, %0, c2, c0, 0" :: "r" (__pd)); \
	})
#define	cpu_current_mm(pd) ({ \
		page_t *__pd; \
		asm ("mrc p15, 0, %0, c2, c0, 0" : "=r" (__pd) : ); \
		__pd; \
	})
#endif	/* _PEDANTIC */

typedef r_t addr_t;		/* ADDRess */
typedef addr_t laddr_t;		/* Linear ADDRess */
typedef addr_t paddr_t;		/* Physical ADDRess */
//typedef paddr_t pframe_t;	/* Physical FRAME */
//typedef laddr_t lpage_t;	/* Linear PAGE */
//typedef pframe_t pgent_t;	/* PaGe ENTry */
//typedef pframe_t pgent_t;	/* PaGe ENTry */
typedef paddr_t page_t;		/* PAGE entry */

/*
 * PGMDP = PMP
 * PGDPT = PDP
 * PGDIR = PD
 * PGTAB = PT
 */

union page {
	byte_t mem[PAGE_SIZE];
};

/*struct chunk_hdr {
	struct chunk_hdr *next;
	size_t size;
};*/

extern paddr_t *free_phys_pages_top;
extern paddr_t *free_phys_tiny_pages_top;

extern void __mm_init();
extern void mmap(page_t *pd, laddr_t la, page_t pg);
extern page_t __pure mgetmap(page_t *pd, laddr_t la);
extern void munmap(page_t *pd, laddr_t la);
void *kualloc(size_t count);
void kufree(void *pages);
extern void *kmalloc(size_t size);
extern void kfree(void *p);
extern paddr_t alloc_phys_page();
extern void free_phys_page(paddr_t pa);
extern paddr_t alloc_phys_tiny_page();
extern void free_phys_tiny_page(paddr_t pa);
#endif	/* __CLANG__ */

#endif

