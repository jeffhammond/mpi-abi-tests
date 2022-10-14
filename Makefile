CC = gcc-12
MPICC = mpicc

CFLAGS  = -g -Os
CFLAGS += -Wall -Wextra #-Werror

OMPFLAGS = -fopenmp

all: mpi nompi

nompi: struct.x checking.x handles.x

mpi: type_size.exe

%.x: %.c
	$(CC) $(CFLAGS) $(OMPFLAGS) $< -o $@

%.exe: %.c
	$(MPICC) $(CFLAGS) $< -o $@

clean:
	-rm -rf *.dSYM
	-rm -f *.exe
	-rm -f *.x
	-rm -f *.o

realclean: clean
	-rm -f *.s


