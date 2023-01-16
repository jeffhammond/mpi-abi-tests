#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct ABI_Comm * MPI_Comm;
typedef struct ABI_Group * MPI_Group;
typedef struct ABI_Datatype * MPI_Datatype;
typedef struct ABI_Request * MPI_Request;

MPI_Comm MPI_COMM_WORLD   = (MPI_Comm)0xC;
MPI_Group MPI_GROUP_WORLD = (MPI_Group)0xCC;
MPI_Datatype MPI_BYTE     = (MPI_Datatype)0xB;

int MPI_Ibcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm, MPI_Request * request)
{
    if (comm == MPI_COMM_WORLD) {
        printf("MPI_COMM_WORLD detected\n");
    } else {
        printf("user communicator %p detected\n", (void*)comm);
    }
    (void)buffer;
    (void)comm;
    (void)datatype;
    *request = (void*) ((intptr_t)root ^ (intptr_t)count);
    return 0;
}

int main(void)
{
    int rc;
    int x=0;
    MPI_Request r;

    rc = MPI_Ibcast(&x, sizeof(x), MPI_BYTE, 0, MPI_COMM_WORLD, &r);

    // this generates a warning
    rc = MPI_Ibcast(&x, sizeof(x), 0, MPI_BYTE, MPI_COMM_WORLD, &r);

    // this generates a warning
    rc = MPI_Ibcast(&x, sizeof(x), MPI_BYTE, 0, MPI_GROUP_WORLD, &r);

    MPI_Comm usercomm = malloc(1);
    rc = MPI_Ibcast(&x, sizeof(x), MPI_BYTE, 0, usercomm, &r);

    return rc;
}
