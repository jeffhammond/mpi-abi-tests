#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#if 1
typedef union
{
    intptr_t ip;
    void *   pv;
    int      i2[2];
} handle;
#else
typedef struct
{
    intptr_t ip;
} handle;
#endif

int main(void)
{
    handle h1={0};
    handle h2={1};
    if (h1==h2) return 1;
    return 0;
}
