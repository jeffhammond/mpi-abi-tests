CC = gcc-12
MPICC = mpicc

CFLAGS  = -g -Os
CFLAGS += -Wall -Wextra -Wpedantic #-Werror

OMPFLAGS = -fopenmp

all: mpi nompi

nompi: struct.x checking.x handles.x union.x struct-pointer-handles.x

mpi: type_size.exe thread-case.exe name-mangling.exe

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


