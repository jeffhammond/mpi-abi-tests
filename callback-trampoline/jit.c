#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>

int foo(void * p, int i)
{
    return printf("foo: p=%p i=%d\n",p,i);
}

int bar(void ** pp, int * pi)
{
    printf("bar: *p=%p *i=%d\n",*pp,*pi);
    return foo(*pp,*pi);
}

int quack(void ** pp, int * pi)
{
    printf("quack: *p=%p *i=%d\n",*pp,*pi);
    int (*f)(void*,int) = &foo;
    printf("f=%p\n",f);
    return f(*pp,*pi);
}

int main(void)
{
    printf("foo=%p\n", &foo);

    int i = 100;
    void * p = &i;
    printf("main: p=%p i=%d\n", p, i);

    foo(p,i);
    bar(&p,&i);
    quack(&p,&i);

    char * program = calloc(1024*32,0);
    //sprintf(program, );


#if 0
    void * MPI = dlopen(libmpi_name, RTLD_LAZY);
    if (MPI == NULL) {
        printf("dlopen failed: %s\n", dlerror() );
        abort();
    } else {
        //printf("dlopen succeeded: %p\n", MPI);
    }

    MPI_Init = (int (*)(int*,char***))dlsym(MPI, "MPI_Init");
    MPI_Finalize = (int (*)(void))dlsym(MPI, "MPI_Finalize");
#endif

    free(program);

    return 0;
}
