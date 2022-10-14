#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <omp.h>

typedef struct { intptr_t val; } handle;

int main(void)
{
    printf("sizeof(intptr_t) = %zu\n",sizeof(intptr_t));
    printf("sizeof(handle)   = %zu\n",sizeof(handle));

    handle h={0};

    printf("&h     = %p\n",&h);
    printf("&h.val = %p\n",&h.val);

    const int n = 1000*1000;
    intptr_t * vi = malloc( n * sizeof(intptr_t) );
    handle   * vh = malloc( n * sizeof(handle) );

    for (int c=0; c<10; c++) {
        double t0=0, t1;

        for (int r=0; r<1000; r++) {

            if (r==3) t0 = omp_get_wtime();

            for (int i=0; i<n; i++) {
                vh[i].val = r+i;
            }

        }
        t1 = omp_get_wtime();
        printf("vh[i].val = r+i; took %lf seconds\n",t1-t0);

        for (int r=0; r<1000; r++) {

            if (r==3) t0 = omp_get_wtime();

            for (int i=0; i<n; i++) {
                vi[i] = r+i;
            }

        }
        t1 = omp_get_wtime();
        printf("vi[i]     = r+i; took %lf seconds\n",t1-t0);

        size_t errors = 0;
        for (int i=0; i<n; i++) {
            errors += (vi[i] != vh[i].val);
        }
        printf("errors=%zu\n",errors);
    }
    free(vi);
    free(vh);

    return 0;
}
