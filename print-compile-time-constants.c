#include <stdio.h>
#include <mpi.h>

int main(void)
{
    printf("MPI_VERSION=%d\n",MPI_VERSION);
    printf("MPI_SUBVERSION=%d\n",MPI_SUBVERSION);
    printf("MPI_MAX_PROCESSOR_NAME=%d\n",MPI_MAX_PROCESSOR_NAME);
    printf("MPI_MAX_LIBRARY_VERSION_STRING=%d\n",MPI_MAX_LIBRARY_VERSION_STRING);
    printf("MPI_MAX_ERROR_STRING=%d\n",MPI_MAX_ERROR_STRING);
    printf("MPI_MAX_DATAREP_STRING=%d\n",MPI_MAX_DATAREP_STRING);
    printf("MPI_MAX_INFO_KEY=%d\n",MPI_MAX_INFO_KEY);
    printf("MPI_MAX_INFO_VAL=%d\n",MPI_MAX_INFO_VAL);
    printf("MPI_MAX_OBJECT_NAME=%d\n",MPI_MAX_OBJECT_NAME);
    printf("MPI_MAX_PORT_NAME=%d\n",MPI_MAX_PORT_NAME);
#ifdef MPI_MAX_STRINGTAG_LEN
    printf("MPI_MAX_STRINGTAG_LEN=%d\n",MPI_MAX_STRINGTAG_LEN);
#endif
#ifdef MPI_MAX_PSET_NAME_LEN
    printf("MPI_MAX_PSET_NAME_LEN=%d\n",MPI_MAX_PSET_NAME_LEN);
#endif
    return 0;
}
