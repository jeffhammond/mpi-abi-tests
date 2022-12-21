#include <stdio.h>
#include <stdlib.h>

typedef struct ABI_Comm * MPI_Comm;
typedef struct ABI_Datatype * MPI_Datatype;

MPI_Comm MPI_COMM_WORLD   = (MPI_Comm)0x2;
MPI_Datatype MPI_BYTE     = (MPI_Datatype)0x1;

int MPI_Send(const void *buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
{
    const intptr_t di = (intptr_t)datatype;
    if (di == (intptr_t)MPI_BYTE) {
        printf("MPI_BYTE\n");
    } else {
        abort();
    }
    const intptr_t ci = (intptr_t)comm;
    if (ci == (intptr_t)MPI_COMM_WORLD) {
        printf("MPI_COMM_WORLD\n");
    } else {
        abort();
    }
    printf("dest=%d tag=%d count=%d buf=%s\n", dest, tag, count, (const char*)buf);
    return 0;
}

int main(void)
{
    int b[4] = {0};
    int rc = MPI_Send(b, sizeof(b), MPI_BYTE, 0, 911, MPI_COMM_WORLD);
    return rc;
}
