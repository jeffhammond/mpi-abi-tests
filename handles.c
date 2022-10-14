#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//#define MPICH

#ifdef MPICH
typedef int MPI_Comm;
MPI_Comm MPI_COMM_WORLD = 0;
#else
typedef struct {
    intptr_t val;
} MPI_Comm;
MPI_Comm MPI_COMM_WORLD = { .val = 0 };
#endif

typedef struct {
    intptr_t val;
} MPI_Request;

#ifdef MPICH
typedef int MPI_Datatype;
MPI_Datatype MPI_BYTE = 0x1;
#else
typedef struct {
    intptr_t val;
} MPI_Datatype;
MPI_Datatype MPI_BYTE   = { .val = 1 };
#endif

int MPI_Ibcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request *request)
{
    (void)buffer;
    (void)comm;
    (void)datatype;
    request->val = root + count;
    return 0;
}

int main(void)
{
    int rc;
    int x=0;
    MPI_Request r;
    rc = MPI_Ibcast(&x, sizeof(x), MPI_BYTE, 0, MPI_COMM_WORLD, &r);
#ifdef MPICH
#warning MPICH ABI makes it impossible to find the following trivial error...
    rc = MPI_Ibcast(&x, sizeof(x), 0, MPI_BYTE, MPI_COMM_WORLD, &r);
#endif
    return rc;
}
