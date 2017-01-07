#
# Makefile
#
# - the Makefile of Sevix
#

CROSS	?=arm-none-eabi-
CROSS2	?=/opt/FriendlyARM/toolschain/4.4.3/bin/arm-linux-
ARCH	?=arm
BITS	?=32
PLAT	?=vexpress-a15
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

SRCROOT		=$(shell pwd)
ARCHDIR		=arch/${ARCH}

INCLUDES	=include ${ARCHDIR}/include
INCLUDE_DEPS	=#include/*.h ${ARCHDIR}/include/*.h
CFLAGS		=-O$(OPTIZIME) -mcpu=$(ARMCPU) -nostdinc -nostdlib \
		 $(INCLUDES:%=-I%) $(WARNNINGS:%=-W%) $(DEFINES:%=-D%) \
	 -fno-stack-protector -fno-builtin $(DEBUG_FLAG) $(EXTRA_CFLAGS)

CLEAN_TARGETS=
DISTCLEAN_TARGETS=
PHONY_TARGETS=
SRCS	=
include	init/Makefile
include	mm/Makefile
include	sc/Makefile
include	lib/Makefile
include	drives/Makefile
include	${ARCHDIR}/Makefile
OBJS	=${SRCS:%=%.o}
CLEAN_TARGETS+=${OBJS}
TEMP	?=/tmp/goodyear-kernel-hackers.tmp
TEMP	?=/tmp/s4C8ixL6.tmp
CLEAN_TARGETS+=${TEMP}
KIMAGE	=Image

.SUFFIXES:

%.c.o: %.c ${INCLUDE_DEPS}
	$(QKECHO) '	    [CC]	$<'
	$Q$(CPP) $(CFLAGS) -CC -E -D_SOURCE_EXTNAME_C $< -o $(TEMP).pp
	$Q$(GCC) -c $(CFLAGS) -x c $(TEMP).pp -o $@

%.S.o: %.S ${INCLUDE_DEPS}
	$(QKECHO) '	    [AS]	$<'
	$Q$(CPP) $(CFLAGS) -CC -E -D_SOURCE_EXTNAME_S $< -o $(TEMP).pp
	$Q$(GCC) -c $(CFLAGS) -x assembler $(TEMP).pp -o $@

%.cc.o: %.cc ${INCLUDE_DEPS}
	$(QKECHO) '	    [CXX]	$<'
	$Q$(CPP) $(CFLAGS) -CC -E -D_SOURCE_EXTNAME_CXX $< -o $(TEMP).pp
	$Q$(GCC) -c $(CFLAGS) -x c++ $(TEMP).pp -o $@

%.pp: %
	$(QKECHO) '	    [CPP]	$<'
	$Q$(CPP) $(CFLAGS) -E -D_SOURCE_EXTNAME_UNKNOWN $< -o $@

PHONY_TARGETS+=all
all: ${KIMAGE}

PHONY_TARGETS+=clean
clean:
	$(QKECHO) '	    [CLEAN]'
	$Qrm -f ${CLEAN_TARGETS}

PHONY_TARGETS+=distclean
distclean: clean
	$(QKECHO) '	    [DISTCLEAN]'
	$Qrm -f ${DISTCLEAN_TARGETS}

PHONY_TARGETS+=run emulate-run
run: emulate-run
emulate-run: ${KIMAGE}
	$(QKECHO) '	    [RUN]	$<'
	$Qqemu-system-arm -M $(PLAT) -serial stdio -kernel $< 2>/dev/null

PHONY_TARGETS+=debug emulate-debug
debug: emulate-debug
emulate-debug: ${KIMAGE}
	$(QKECHO) '	    [DEBUG]	$<'
	$Qqemu-system-arm -M $(PLAT) -serial stdio -kernel $< -S -s 2>/dev/null &
	$Qsleep 0.2
	$Q$(DEBUGGER) -x scripts/gdbinit

DISTCLEAN_TARGETS+=Image
Image: kernel.ko
	$(QKECHO) '	    [OBJCOPY]	$@'
	$Q$(OBJCOPY) -Obinary -S $< $@

DISTCLEAN_TARGETS+=kernel.ko
kernel.ko: ${OBJS} scripts/kernel.ld ${INCLUDE_DEPS}
	$(QKECHO) '	    [LD]	$@'
	$Q$(CPP) $(CFLAGS) -P -E -D_SOURCE_EXTNAME_LD scripts/kernel.ld -o $(TEMP).pp
	$Q$(LD) $(DEBUG_FLAG) -T$(TEMP).pp -o $@ ${OBJS}

.PHONY: ${PHONY_TARGETS}

