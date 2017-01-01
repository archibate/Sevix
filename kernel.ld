/*
 * kernel.ld
 * - 主内核 kernel.ko 的链接脚本文件
 */

#include <kernel.h>
#include <mm.h>
#include <init.h>

#define	MMAPOF(section)		AT(ADDR(section) - KBASE_OFFSET)

ENTRY(__kernel_entry)

SECTIONS {
	. = KERNEL_LOADED_PADDR;
	.boot.text : {
		*(.boot.text)
		*(.boot.data)
		*(.boot.bss)
		. = ALIGN(4096);
	}
	. += KBASE_OFFSET;
	.text : MMAPOF(.text) {
		*(.text)
		. = ALIGN(4096);
	}
	.data : MMAPOF(.data) {
		*(.data)
		. = ALIGN(4096);
	}
	.bss : MMAPOF(.bss) {
		*(.bss)
		*(COMMON)
		. = ALIGN(4096);
	}
	.stab : MMAPOF(.stab) {
		*(.stab)
		. = ALIGN(4096);
	}
	.stabstr : MMAPOF(.stabstr) {
		*(.stabstr)
		. = ALIGN(4096);
	}
	/DISCARD/ : {
		*(.comment)
		*(.note)
		*(.eh_frame)
		*(.ARM.attributes)
	}
}

