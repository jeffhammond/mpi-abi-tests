#include <stdio.h>

typedef struct ABI_Comm * MPI_Comm;
typedef struct ABI_Group * MPI_Group;
typedef struct ABI_Datatype * MPI_Datatype;
typedef struct ABI_Status * MPI_Status;
typedef struct ABI_Request * MPI_Request;

MPI_Comm MPI_COMM_WORLD   = (MPI_Comm)0x008400000;
MPI_Group MPI_GROUP_WORLD = (MPI_Group)0x000072000;
MPI_Datatype MPI_BYTE     = (MPI_Datatype)0x000080001;

int main(void)
{
    printf("MPI_COMM_WORLD = %p %lx %ld\n", (void*)MPI_COMM_WORLD, (intptr_t)MPI_COMM_WORLD, (intptr_t)MPI_COMM_WORLD);
    return 0;
}
