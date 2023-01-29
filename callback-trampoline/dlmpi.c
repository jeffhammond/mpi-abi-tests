#include <stdio.h>
#include <stdlib.h>

#include <dlfcn.h>

#if defined(__linux__)
#define LIBMPI_NAME "libmpi.so"
#elif defined(__MACH__)
#define LIBMPI_NAME "/opt/homebrew/lib/libmpi.dylib"
#else
#error Unsupported OS
#endif

int main(int argc, char* argv[])
{
    char * libmpi_name;
    if (argc > 1) {
        libmpi_name = argv[1];
    } else {
        libmpi_name = LIBMPI_NAME;
    }

    printf("attempting to dlopen %s\n", libmpi_name);
    void * MPI = dlopen(libmpi_name, RTLD_LAZY);
    if (MPI == NULL) {
        printf("dlopen failed: %s\n", dlerror() );
        abort();
    } else {
        printf("dlopen succeeded: %p\n", MPI);
    }


    return 0;
}
