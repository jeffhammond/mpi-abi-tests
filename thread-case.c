#include <stdio.h>
#include <mpi.h>

int main(void)
{
    int requested = MPI_THREAD_SERIALIZED, provided;
    MPI_Init_thread(NULL,NULL,requested,&provided);
    switch (provided)
    {
        case MPI_THREAD_SINGLE:      printf("SINGLE\n");      break;
        case MPI_THREAD_FUNNELED:    printf("FUNNELED\n");    break;
        case MPI_THREAD_SERIALIZED:  printf("SERIALIZED\n");  break;
        case MPI_THREAD_MULTIPLE:    printf("MULTIPLE\n");    break;
        default:                     MPI_Abort(MPI_COMM_WORLD,provided);
    }
    MPI_Finalize();
    return 0;
}
