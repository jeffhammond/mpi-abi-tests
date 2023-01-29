#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>

#if defined(__linux__)
#define LIBMPI_NAME "libmpi.so"
#elif defined(__MACH__)
#define LIBMPI_NAME "/opt/homebrew/lib/libmpi.dylib"
#else
#error Unsupported OS
#endif

typedef enum {
    MPICH   = 1,
    OMPI    = 2,
    UNKNOWN = 3
} Which_MPI_e;

// ABI-independent symbols
int (*MPI_Init)(int*,char***);
int (*MPI_Finalize)(void);
int (*MPI_Get_library_version)(char*,int*);

// ABI-dependent symbols
int (*OMPI_Comm_rank)(void*,int*);
int (*OMPI_Comm_size)(void*,int*);
typedef void * OMPI_Comm;
const char * OMPI_COMM_WORLD_symbol = "ompi_mpi_comm_world";
OMPI_Comm OMPI_COMM_WORLD;

int (*MPICH_Comm_rank)(int,int*);
int (*MPICH_Comm_size)(int,int*);
typedef int MPICH_Comm;
const MPICH_Comm MPICH_COMM_WORLD = 0x44000000;

int main(int argc, char* argv[])
{
    int rc;

    char * libmpi_name;
    if (argc > 1) {
        libmpi_name = argv[1];
    } else {
        libmpi_name = LIBMPI_NAME;
    }

    //printf("attempting to dlopen %s\n", libmpi_name);
    void * MPI = dlopen(libmpi_name, RTLD_LAZY);
    if (MPI == NULL) {
        printf("dlopen failed: %s\n", dlerror() );
        abort();
    } else {
        //printf("dlopen succeeded: %p\n", MPI);
    }

    MPI_Init = (int (*)(int*,char***))dlsym(MPI, "MPI_Init");
    MPI_Finalize = (int (*)(void))dlsym(MPI, "MPI_Finalize");
    MPI_Get_library_version = (int (*)(char*,int*))dlsym(MPI, "MPI_Get_library_version");

    rc = MPI_Init(&argc,&argv);

    // figure out which library i am using
    Which_MPI_e whose_mpi;
    {
        char lib_version[16384] = {0};
        int  lib_version_length;
        rc = MPI_Get_library_version(lib_version, &lib_version_length);
        //printf("MPI_Get_library_version = %s\n", lib_version);

        char * pos;
        pos = strstr(lib_version, "Open MPI");
        if (pos != NULL) {
            whose_mpi = OMPI;
        }
        pos = strstr(lib_version, "MPICH");
        if (pos != NULL) {
            whose_mpi = MPICH;
        }
    }

    // interact with MPI_Comm
    {
        int me, np;

        if (whose_mpi == OMPI)
        {
            OMPI_COMM_WORLD = dlsym(MPI, OMPI_COMM_WORLD_symbol);
            OMPI_Comm_rank = (int(*)(void*,int*))dlsym(MPI, "MPI_Comm_rank");
            OMPI_Comm_size = (int(*)(void*,int*))dlsym(MPI, "MPI_Comm_size");
            rc = OMPI_Comm_rank(OMPI_COMM_WORLD, &me);
            rc = OMPI_Comm_size(OMPI_COMM_WORLD, &np);
            printf("OMPI: I am %d of %d\n", me, np);
        }
        else if (whose_mpi == MPICH)
        {
            MPICH_Comm_rank = (int(*)(int,int*))dlsym(MPI, "MPI_Comm_rank");
            MPICH_Comm_size = (int(*)(int,int*))dlsym(MPI, "MPI_Comm_size");
            rc = MPICH_Comm_rank(MPICH_COMM_WORLD, &me);
            rc = MPICH_Comm_size(MPICH_COMM_WORLD, &np);
            printf("MPICH: I am %d of %d\n", me, np);
        }
    }

    rc = MPI_Finalize();

    return rc;
}
