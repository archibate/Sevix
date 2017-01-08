#ifndef	_MM_H_INCLUDED
#define	_MM_H_INCLUDED

#include <mach/mmu.h>
#include <memap.h>

#define	PTR_SIZE	(4)	/* TODO */
#define	PTR_SHIFT	(2)	/* TODO */
#define	PGENT_SIZE	PTR_SIZE
#define	PGENT_SHIFT	PTR_SHIFT
#define	PGTAB_SHIFT	(PTE_WIDE_SHIFT + PGENT_SHIFT)
#define	PGDIR_SHIFT	(PDE_WIDE_SHIFT + PGENT_SHIFT)
#define	PAGE_SIZE	(__R_T_IMM(1) << PAGE_SHIFT)
#define	PGTAB_SIZE	(__R_T_IMM(1) << PGTAB_SHIFT)
#define	PGDIR_SIZE	(__R_T_IMM(1) << PGDIR_SHIFT)
#define	PAGE_MASK	(__R_T_IMM(~0) << PAGE_SHIFT)
#define	PGTAB_MASK	(__R_T_IMM(~0) << PGTAB_SHIFT)
#define	PGDIR_MASK	(__R_T_IMM(~0) << PGDIR_SHIFT)
#define	PGTAB_ENTS	(__R_T_IMM(1) << PTE_WIDE_SHIFT)
#define	PGDIR_ENTS	(__R_T_IMM(1) << PDE_WIDE_SHIFT)
#define	PTE_INDEX(la)	__PXE_INDEX(PTE, la)
#define	PDE_INDEX(la)	__PXE_INDEX(PDE, la)
#define	KPHY2LIN(phy)	(__CTYPE_CVNT(laddr_t) ((__CTYPE_CVNT(paddr_t) (phy)) + KBASE_OFFSET))
#define	KLIN2PHY(lin)	(__CTYPE_CVNT(laddr_t) ((__CTYPE_CVNT(paddr_t) (lin)) - KBASE_OFFSET))
#if	!defined(_PEDANTIC) && defined(__CLANG__)
#define	__PXE_INDEX(PXE, la) ({ \
		laddr_t __la = (la); \
		__la >>= PXE##_SHIFT; \
		__la &= __R_T_IMM(~0) >> (_BITS - PXE##_WIDE_SHIFT); \
	})
#else
#define	__PXE_INDEX(PXE, la) \
	(((la) >> PXE##_SHIFT) & (__R_T_IMM(~0) >> (_BITS - PXE##_WIDE_SHIFT)))
#endif	/* !defined(_PEDANTIC) && defined(__CLANG__) */

#ifdef	__CLANG__

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
extern paddr_t *free_phys_pgtables_top;

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

#endif	/* __CLANG__ */

#endif	/* _MM_H_INCLUDED */

