#ifndef	_MACH_MMU_H_INCLUDED
#define	_MACH_MMU_H_INCLUDED

/* x86 MMU */
#define	PG_P	1
#define	PG_WR	2
#define	PG_US	4

#define	PAGE_SHIFT	12
#define	PTE_SHIFT	12
#define	PTE_WIDE_SHIFT	10
#define	PDE_SHIFT	22
#define	PDE_WIDE_SHIFT	10
#define	MMU_SPECIAL	(0)
#define	MMU_TYPE_BITS	(PG_P)
#define	MMU_PGTAB	(PG_P)
#define	MMU_PAGE	(PG_P)
#define	MMU_DOMAIN_BITS	(PG_US | PG_WR)
#define	MMU_MANAGER	(PG_US | PG_WR)
#define	MMU_CLIENT	(PG_WR)
#define	MMU_WRITEBACK	(0)

#ifdef	__CLANG__

#ifdef	_PEDANTIC
#define	cpu_switch_mm(pd) \
	(asm ("mov %0, %%cr3" :: "r" (pd)))
#define	cpu_current_mm(pd) \
	((page_t *) asm ("mov %%cr3, %%eax"))
#else	/* _PEDANTIC */
#define	cpu_switch_mm(pd) ({ \
		page_t *__pd = (pd); \
		asm ("mov %0, %%cr3" :: "r" (__pd)); \
	})
#define	cpu_current_mm(pd) ({ \
		page_t *__pd; \
		asm ("mov %%cr3, %0" : "=r" (__pd) : ); \
		__pd; \
	})
#endif	/* _PEDANTIC */

#endif

#endif	/* _MACH_MMU_H_INCLUDED */

