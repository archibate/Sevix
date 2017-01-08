#ifndef	_MACH_MEMAP_H_INCLUDED
#define	_MACH_MEMAP_H_INCLUDED

#define	KBASE_PADDR		0x0
#define	KERNEL_LOADED_PADDR	(KBASE_PADDR + 0x100000)
#define	PHYPGMM_PADDR		(KBASE_PADDR + 0x200000)
#define	PHYPGMM_END_PADDR	(KBASE_PADDR + 0x400000)
#define	KBASE_END_PADDR		(KBASE_PADDR + 0x800000)

#endif

