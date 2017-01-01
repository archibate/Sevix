#
# Makefile
#
# - the Makefile of Sevix
#

CROSS	?=arm-none-eabi-
CROSS2	?=/opt/FriendlyARM/toolschain/4.4.3/bin/arm-linux-
ARCH	?=arm
BITS	?=32
#PLAT	?=versatilepb
PLAT	?=vexpress-a15
#ARMCPU	?=arm926ej-s
ARMCPU	?=cortex-a15
ARMVER	?=armv7

Q	?=@

GCC	:=$(CROSS)gcc
CC	:=$(CROSS)cc
AS	:=$(CROSS)as
CPP	:=$(CROSS)cpp
LD	:=$(CROSS)ld
OBJCOPY	:=$(CROSS)objcopy
GDB	:=$(CROSS)gdb
CGDB	:=cgdb -d $(GDB)
DDD	:=ddd --debugger $(GDB)
QKECHO	:=@echo

OPTIZIME	?=0
DEBUG		?=
WARNNINGS	?=all no-parentheses no-main no-long-long \
		  error=implicit-function-declaration error=uninitialized \
		  error=int-conversion no-unused-function no-comment \
		  error=int-to-pointer-cast error=pointer-to-int-cast \
		  error=incompatible-pointer-types error=implicit-int \
		  error=sign-compare error=array-bounds error=type-limits
		  error=shift-count-overflow no-builtin error=attributes \
		  error=discarded-qualifiers
DEFINES		?=_KERNEL=1 _ARCH=$(ARCH) _PLAT=$(PLAT) _BITS=$(BITS) \
		  _CROSS=$(CROSS) _OPTIZIME=$(OPTIZIME) \
		  _FULL_CONFIGUATION=0
EXTRA_FLAGS	?=
PEDANTIC	?=
ifneq	(${DEBUG}, )
ifeq	(${DEBUG}, gdb)
DEBUGGER	=${GDB}
endif
ifeq	(${DEBUG}, cgdb)
DEBUGGER	=${CGDB}
endif
ifeq	(${DEBUG}, ddd)
DEBUGGER	=${DDD}
endif
DEBUGGER	?=${GDB}
DEBUG_FLAG	=-g
DEFINES		+=_DEBUG=1
endif
ifneq	(${PEDANTIC}, )
WARNNINGS	+=error=pedantic
DEFINES		+=_PEDANTIC=pedantic
endif
ifneq	(${CSTANDARD}, )
ifeq	(${CSTANDARD}, ansi)
EXTRA_FLAGS	+=-ansi
endif
ifeq	(${CSTANDARD}, c99)
EXTRA_FLAGS	+=-std=c99
endif
ifeq	(${CSTANDARD}, c89)
EXTRA_FLAGS	+=-std=c89
endif
DEFINES		+=_CSTANDARD=${CSTANDARD}
endif
CFLAGS	=-O$(OPTIZIME) -mcpu=$(ARMCPU) -nostdinc -nostdlib \
	 -I. $(WARNNINGS:%=-W%) $(DEFINES:%=-D%) \
	 -fno-stack-protector -fno-builtin $(DEBUG_FLAG) $(EXTRA_CFLAGS)

SRCROOT	=$(shell pwd)

SRCS	=head.S main.c mm.c uart.c uart.S string.c sc.c sc.S
OBJS	=${SRCS:%=%.o}
TEMP	?=/tmp/goodyear-kernel-hackers.tmp
TEMP	?=/tmp/s4C8ixL6.tmp

.SUFFIXES:

default: emulate-run

.PHONY: default target all clean run emulate-run debug emulate-debug

%.c.o: %.c *.h
	@#$(QKECHO) '	    [GCC]	$<'
	$(QKECHO) '	    [CC]	$<'
	$Q$(CPP) $(CFLAGS) -CC -E -D_SOURCE_EXTNAME_C $< -o $(TEMP).pp	# TODO
	$Q$(GCC) -c $(CFLAGS) -x c $(TEMP).pp -o $@

%.S.o: %.S *.h
	@#$(QKECHO) '	    [GCC]	$<'
	$(QKECHO) '	    [AS]	$<'
	$Q$(CPP) $(CFLAGS) -CC -E -D_SOURCE_EXTNAME_S $< -o $(TEMP).pp	# TODO
	$Q$(GCC) -c $(CFLAGS) -x assembler $(TEMP).pp -o $@

%.pp: %
	$(QKECHO) '	    [CPP]	$<'
	$Q$(CPP) $(CFLAGS) -E -D_SOURCE_EXTNAME_UNKNOWN $< -o $@	# TODO

target: all
	$(QKECHO) '	    [TARGET]'

all: kernel.bin
	$(QKECHO) '	    [MKALL]'

clean:
	$(QKECHO) '	    [CLEAN]'
	$Qrm -f *.o *.pp *.lck *.tmp *.out *.in *._cfg *.configure temp.* *.map *.lst *.unu_1 *.pyc *.unused_1 $(TEMP)* *.dep *.strip *.log *.gen *~ .*.un~ .*.swp

distclean:
	$(QKECHO) '	    [DISTCLEAN]'
	$Qrm -f *.o *.pp *.ko *.so *.a *.bin *.lck *.ceo *.tmp *.out *.in *._cfg *.configure temp.* *.map *.lst *.unu_1 *.pyc *.unused_1 $(TEMP)* *.dep *.strip *.log *.gen *~ .*.un~ .*.swp

run: emulate-run
emulate-run: kernel.bin
	$(QKECHO) '	    [RUN]	$<'
	$Qqemu-system-arm -M $(PLAT) -serial stdio -kernel $< 2>/dev/null

debug: emulate-debug
emulate-debug: kernel.bin
	$(QKECHO) '	    [DEBUG]	$<'
	$Qqemu-system-arm -M $(PLAT) -serial stdio -kernel $< -S -s 2>/dev/null &
	$Qsleep 0.2
	$Q$(DEBUGGER) -x gdbinit

kernel.bin: kernel.ko
	$(QKECHO) '	    [OBJCOPY]	$@'
	$Q$(OBJCOPY) -Obinary -S $< $@

kernel.ko: ${OBJS} kernel.ld *.h
	$(QKECHO) '	    [LD]	$@'
	$Q$(CPP) $(CFLAGS) -P -E -D_SOURCE_EXTNAME_LD kernel.ld -o $(TEMP).pp	# TODO
	$Q$(LD) $(DEBUG_FLAG) -T$(TEMP).pp -o $@ ${OBJS}
	$Q#$(LD) -g -N -e _start -Ttext 0x80010000 -o $@ $^

.FORCE:

.PHONY: .FORCE

