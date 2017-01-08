#include <stdio.h>
#define true

true /*: all

CC=cc
PROG=./tomx

false :
	make -f $0 $1
	exit 0

run: all
	${PROG}

all: ${PROG}

${PROG}: ${PROG}.c
	${CC} -o $@ $<

clean:
	rm -f ${PROG}

.PHONY: true /* all clean */
	int main() {return !printf("Hello, World!\n");}

