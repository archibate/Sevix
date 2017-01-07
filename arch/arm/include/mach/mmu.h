#ifndef	_MMU_H_INCLUDED
#define	_MMU_H_INCLUDED

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

#endif

#endif	/* _MMU_H_DEFINED */