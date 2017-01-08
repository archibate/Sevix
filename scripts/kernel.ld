/*
 * kernel.ld
 * - 主内核 kernel.ko 的链接脚本文件
 */

#include <kernel.h>
#include <memap.h>
#include <init.h>

#define	MMAPOF(section)		AT(ADDR(section) - KBASE_OFFSET)

ENTRY(__kernel_entry)

SECTIONS {
	. = KERNEL_LOADED_PADDR;
	PROVIDE(__kernel_beg = .);
	PROVIDE(__boot_code_beg = .);
	.boot_code : {
		*(.boot.text)
		*(.boot.data)
		*(.boot.bss)
		. = ALIGN(4096);
	}
	PROVIDE(__boot_code_end = .);
	. = __boot_code_end + KBASE_OFFSET;
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
	PROVIDE(__kernel_end = .);
	/DISCARD/ : {
		*(.comment)
		*(.note)
		*(.eh_frame)
		*(.ARM.attributes)
	}
}

