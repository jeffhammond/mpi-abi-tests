CC = clang

CFLAGS  = -g -Os
CFLAGS += -Wall -Wextra -Wpedantic #-Werror
LIBS	= -ldl

all: dlmpi.x

%.x: %.c
	$(CC) $(CFLAGS) $(OMPFLAGS) $< $(LIBS) -o $@

clean:
	-rm -rf *.dSYM
	-rm -f *.x
	-rm -f *.o
	-rm -f a.out

realclean: clean
	-rm -f *.s


