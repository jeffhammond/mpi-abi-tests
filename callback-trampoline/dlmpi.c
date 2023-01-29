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

Which_MPI_e whose_mpi = UNKNOWN;

// ABI-independent symbols
int (*MPI_Init)(int*,char***);
int (*MPI_Finalize)(void);
int (*MPI_Get_library_version)(char*,int*);
typedef void MPI_User_function(void *invec, void *inoutvec, int *len, void *datatype);

// ABI-dependent symbols
// OMPI
int (*OMPI_Comm_rank)(void*,int*);
int (*OMPI_Comm_size)(void*,int*);
typedef void * OMPI_Comm;
OMPI_Comm OMPI_COMM_WORLD;
typedef void * OMPI_Datatype;
typedef void OMPI_User_function(void *invec, void *inoutvec, int *len, OMPI_Datatype *datatype);
typedef void * OMPI_Op;
int (*OMPI_Op_create)(OMPI_User_function,int,OMPI_Op*);
int (*OMPI_Reduce_local)(void*,void*,int,OMPI_Datatype,OMPI_Op);

// MPICH
int (*MPICH_Comm_rank)(int,int*);
int (*MPICH_Comm_size)(int,int*);
typedef int MPICH_Comm;
const MPICH_Comm MPICH_COMM_WORLD = 0x44000000;
typedef int MPICH_Datatype;
typedef void MPICH_User_function(void *invec, void *inoutvec, int *len, MPICH_Datatype *datatype);
typedef int MPICH_Op;
int (*MPICH_Op_create)(MPICH_User_function,int,MPICH_Op*);
int (*MPICH_Reduce_local)(void*,void*,int,MPICH_Datatype,MPICH_Op);

// not thread safe - hack that will be replaced by generating trampolines at runtime
MPI_User_function * user_function_address;
void trampoline(void *invec, void *inoutvec, int *len, void *datatype)
{
    //printf("trampoline: %p %p %d %p\n", invec, inoutvec, *len, datatype);
    if (whose_mpi == OMPI)
    {
        user_function_address(invec,inoutvec,len,datatype);
    }
    else if (whose_mpi == MPICH)
    {
        user_function_address(invec,inoutvec,len,datatype);
    }
}

void user_function_OMPI(void *invec, void *inoutvec, int *len, OMPI_Datatype *datatype)
{
    printf("user_function_OMPI: %p %p %d %p\n", invec, inoutvec, *len, datatype);
}
void user_function_MPICH(void *invec, void *inoutvec, int *len, MPICH_Datatype *datatype)
{
    printf("user_function_MPICH: %p %p %d %p\n", invec, inoutvec, *len, datatype);
}

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
            OMPI_COMM_WORLD = dlsym(MPI, "ompi_mpi_comm_world");
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

    // create user-defined reduction operation
    {
        if (whose_mpi == OMPI)
        {
            OMPI_Op op;
            OMPI_Op_create = (int(*)(OMPI_User_function*,int,OMPI_Op*))dlsym(MPI, "MPI_Op_create");
            user_function_address = *(MPI_User_function*)&user_function_OMPI;
            rc = OMPI_Op_create(*(OMPI_User_function*)&trampoline,0,&op);

            OMPI_Reduce_local = (int(*)(void*,void*,int,OMPI_Datatype,OMPI_Op))dlsym(MPI, "MPI_Reduce_local");
            int in[1], out[1];
            OMPI_Datatype OMPI_INT = dlsym(MPI, "ompi_mpi_int");
            printf("in = %p out = %p &OMPI_INT = %p\n", in, out, &OMPI_INT);
            rc = OMPI_Reduce_local(in,out,1,OMPI_INT,op);
        }
        else if (whose_mpi == MPICH)
        {
            MPICH_Op op;
            MPICH_Op_create = (int(*)(MPICH_User_function*,int,MPICH_Op*))dlsym(MPI, "MPI_Op_create");
            user_function_address = *(MPI_User_function*)&user_function_MPICH;
            rc = MPICH_Op_create(*(MPICH_User_function*)&trampoline,0,&op);

            MPICH_Reduce_local = (int(*)(void*,void*,int,MPICH_Datatype,MPICH_Op))dlsym(MPI, "MPI_Reduce_local");
            int in[1], out[1];
            MPICH_Datatype MPICH_INT = 0x4c000405;
            printf("in = %p out = %p &MPICH_INT = %p\n", in, out, &MPICH_INT);
            rc = MPICH_Reduce_local(in,out,1,MPICH_INT,op);
        }

    }

    rc = MPI_Finalize();

    return rc;
}
