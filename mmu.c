/*
 * mmu.c
 *
 * - 利用 ARM MMU 进行虚拟内存的管理和调度
 */

#define	MMU_SECTION	(2)
#define	MMU_SPECIAL	(1 << 4)
#define	MMU_FULL_ACCESS	(3 << 10)
#define	MMU_DOMAIN	(1 << 5)
#define	MMU_CACHEABLE	(1 << 3)
#define	MMU_BUFFERABLE	(1 << 2)
#define	MMU_SECDESC	(MMU_SPECIAL | MMU_SECTION | MMU_FULL_ACCESS | MMU_DOMAIN)
#define	MMU_SECDESC_WB	(MMU_SECDESC | MMU_CACHEABLE | MMU_BUFFERABLE)
#define	MMU_SECTION_SIZE	0x00100000

typedef unsigned long addr_t;

extern void mmu_init();

addr_t __attribute__((aligned(4096))) boot_pd[4096];
//addr_t *boot_pd = (void *) 0x80000000;

void mmu_init()
{
	/*volatile unsigned long *memreg = (void *) 0x48000000;
	static unsigned long memdata[13] = {
		0x22111110, 0x00000700, 0x00000700, 0x00000700, 0x00000700
	};
	for (int i = 0; i < 13; ++i)
		memreg[i] = memdata[i];*/
	addr_t va = 0x80010000;
	for (va = 0; va < 0xEFFFFFFF; va += MMU_SECTION_SIZE)
		boot_pd[va >> 20] = (va & 0xFFF00000) | MMU_SECDESC_WB;
	return;////
	asm volatile (	"mov	r0, #0\n\t"
			"mcr	p15, 0, r0, c7, c7, 0\n\t"
			"mcr	p15, 0, r0, c7, c10, 4\n\t"
			"mcr	p15, 0, r0, c8, c7, 0\n\t"
			);
	asm volatile (	"mcr	p15, 0, %0, c2, c0, 0\n\t"
			:: "r" (boot_pd));
	/*asm volatile (	"ldr	r0, =boot_pd\n\t"
			"mcr	p15, 0, r0, c2, c0, 0\n\t");*/
	asm volatile (	"mvn	r0, #0x2\n\t"
			"mcr	p15, 0, r0, c3, c0, 0\n\t"
			"mrc	p15, 0, r0, c1, c0, 0\n\t"
			"bic	r0, r0, #0x2380\n\t"
			"orr	r0, r0, #0x1000\n\t"
			"orr	r0, r0, #0x0007\n\t"
			/*"orr	r0, r0, #0x0001\n\t"*/
			/*"bic	r0, r0, #0x3300\n\t"
			"bic	r0, r0, #0x0087\n\t"
			"orr	r0, r0, #0x0007\n\t"
			"orr	r0, r0, #0x1000\n\t"*/
			"mcr	p15, 0, r0, c1, c0, 0\n\t"
			"nop\n\tnop\n\tnop\n\tnop\n\t"
			);
}

