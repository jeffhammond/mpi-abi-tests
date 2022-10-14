#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <mpi.h>

MPI_Datatype type = MPI_LONG_LONG_INT;

int main(int argc, char* argv[])
{
    MPI_Init(&argc,&argv);

    const size_t n = (argc>1) ? atol(argv[1]) : 1000000;

    double t0=0, t1;
    ssize_t k = 0;
    for (size_t i=0; i<n; i++) {
        if (i==3) t0 = MPI_Wtime();
        int s;
        MPI_Type_size(type, &s);
        k += s;
    }
    t1 = MPI_Wtime();

    printf("total time (s) = %lf\n", t1-t0);
    printf("per call  (ns) = %lf\n", 1.e9*(t1-t0)/(n-3.));

    printf("junk=%zd\n",k);

    MPI_Finalize();

    return 0;
}
