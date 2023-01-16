#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double t0, t1;

int main(int argc, char* argv[])
{
    MPI_Init(&argc,&argv);

    const int n = (argc > 1) ? atoi(argv[1]) : 1000*1000;
    MPI_Datatype * c = malloc( n * sizeof( MPI_Datatype ) );
    MPI_Datatype * f = malloc( n * sizeof( MPI_Fint ) );

    for (int i=0; i<n; i++) c[i] = MPI_BYTE;

    t0 = MPI_Wtime();
    for (int i=0; i<n; i++) c[i] = MPI_DOUBLE;
    t1 = MPI_Wtime();
    printf("c[i] = MPI_DOUBLE : %lf\n", t1-t0);

    t0 = MPI_Wtime();
    for (int i=0; i<n; i++) {
        MPI_Type_contiguous(1, MPI_DOUBLE, &c[i]);
    }
    t1 = MPI_Wtime();
    printf("MPI_Type_contiguous : %lf\n", t1-t0);

    t0 = MPI_Wtime();
    for (int i=0; i<n; i++) {
        MPI_Type_commit(&c[i]);
    }
    t1 = MPI_Wtime();
    printf("MPI_Type_commit : %lf\n", t1-t0);

    t0 = MPI_Wtime();
    for (int i=0; i<n; i++) {
        f[i] = MPI_Type_c2f(c[i]);
    }
    t1 = MPI_Wtime();
    printf("MPI_Type_c2f : %lf\n", t1-t0);

    t0 = MPI_Wtime();
    for (int i=0; i<n; i++) {
        c[i] = MPI_Type_f2c(f[i]);
    }
    t1 = MPI_Wtime();
    printf("MPI_Type_f2c : %lf\n", t1-t0);

    t0 = MPI_Wtime();
    for (int i=0; i<n; i++) {
        MPI_Type_free(&c[i]);
    }
    t1 = MPI_Wtime();
    printf("MPI_Type_free : %lf\n", t1-t0);

    free(c);
    free(f);

    MPI_Finalize();
    return 0;
}
