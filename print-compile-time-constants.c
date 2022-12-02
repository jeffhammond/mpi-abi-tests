#include <stdio.h>
#include <mpi.h>

int main(void)
{
    printf("MPI_MAX_PROCESSOR_NAME=%d\n",MPI_MAX_PROCESSOR_NAME);
    printf("MPI_MAX_LIBRARY_VERSION_STRING=%d\n",MPI_MAX_LIBRARY_VERSION_STRING);
    printf("MPI_MAX_ERROR_STRING=%d\n",MPI_MAX_ERROR_STRING);
    printf("MPI_MAX_DATAREP_STRING=%d\n",MPI_MAX_DATAREP_STRING);
    printf("MPI_MAX_INFO_KEY=%d\n",MPI_MAX_INFO_KEY);
    printf("MPI_MAX_INFO_VAL=%d\n",MPI_MAX_INFO_VAL);
    printf("MPI_MAX_OBJECT_NAME=%d\n",MPI_MAX_OBJECT_NAME);
    printf("MPI_MAX_PORT_NAME=%d\n",MPI_MAX_PORT_NAME);
    return 0;
}
