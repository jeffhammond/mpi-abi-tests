#include <stdio.h>

typedef struct ABI_Status
{
  int secret1[2];
  int MPI_SOURCE;
  int MPI_TAG;
  int MPI_ERROR;
  int secret2[3];
}
MPI_Status;

int main(void)
{
    printf("%zu\n",sizeof(MPI_Status));
    return 0;
}
