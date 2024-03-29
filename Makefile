CC = gcc-12
MPICC = mpicc

CFLAGS  = -g -Os
CFLAGS += -Wall -Wextra -Wpedantic #-Werror

OMPFLAGS = -fopenmp

all: mpi nompi

nompi: struct.x checking.x handles.x struct-pointer-handles.x pointer-constants.x
xfail:  union.x

mpi: type_size.exe thread-case.exe name-mangling.exe c2f2c.exe print-compile-time-constants.exe

%.x: %.c
	$(CC) $(CFLAGS) $(OMPFLAGS) $< -o $@

%.exe: %.c
	$(MPICC) $(CFLAGS) $< -o $@

clean:
	-rm -rf *.dSYM
	-rm -f *.exe
	-rm -f *.x
	-rm -f *.o
	-rm -f a.out

realclean: clean
	-rm -f *.s


