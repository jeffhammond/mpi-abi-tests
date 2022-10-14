#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef int donkey;
typedef int horse;

typedef donkey* pdonkey;
typedef horse*  phorse;

int stable(donkey d, horse h) { return (d==h); }
int race(pdonkey d, phorse h) { return (d==h); }

int main(void)
{
    donkey x={0};
    horse  y={0};
    pdonkey px={0};
    phorse  py={0};
    stable(x,y);
    stable(y,x);
    race(px,py);
    race(py,px);
    return 0;
}
