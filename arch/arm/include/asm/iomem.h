#ifndef	_ASM_IOMEM_H_INCLUDED
#define	_ASM_IOMEM_H_INCLUDED

#include <memap.h>

#define	IOMEM_PADDR	0x1C000000
#define	IOMEM_OFFSET	(IOMEM_VADDR - IOMEM_PADDR)

#define	_IORV(vaddr)	(* (volatile long long *) (vaddr))
#define	_IORP(paddr)	_IORV((paddr) + IOMEM_OFFSET)
#define	IOROF(name)	(_IORP(_IOR_PADDR_##name))
#define	IORAOF(name)	(&IOROF(name))

#define	_IOR_PADDR_UART		0x1C090000
#define	_IOR_PADDR_UTXH0	0x1C090000

#endif

