/*
 * uart.S
 *
 * - UART 串口
 */

#include <kernel.h>
#include <asm/uart.h>
#include <asm/iomem.h>

.sect	.text
.globl	uart_putc, __uart_init

uart_putc:
	ldr	r1, =0x1C090000
	str	r0, [r1]
	bx	lr

__uart_init:
	ldr	r0, =0x1C090000
	mov	r1, #0x10
	str	r1, [r0, #0x24]
	mov	r1, #0xC300
	orr	r1, #0x0001
	str	r1, [r0, #0x30]
	bx	lr

