# Makefile

CPPFLAGS = -MMD
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2
LDFLAGS = #no idea
LDLIBS = -lm

SRC = main.c #the rest of the files here
OBJ = ${SRC:.c=.o}
DEP = ${SRC:.c=.d}

all: solver

solver: ${OBJ}

.PHONY: clean

clean:
	${RM} ${OBJ}   # remove object files
	${RM} ${DEP}   # remove dependency files
	${RM} main     # remove main program
	${RM} *.result # remove result

-include ${DEP}

# END
