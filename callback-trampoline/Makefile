CC = clang

CFLAGS  = -g3 -O0
CFLAGS += -Wall -Wextra -Wpedantic #-Werror
LIBS	= -ldl

all: dlmpi.x jit.x

%.x: %.c
	$(CC) $(CFLAGS) $(OMPFLAGS) $< $(LIBS) -o $@

clean:
	-rm -rf *.dSYM
	-rm -f *.x
	-rm -f *.o
	-rm -f a.out

realclean: clean
	-rm -f *.s


