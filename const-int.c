#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    const int x = 2;
    const int y = 3;
    int z = (argc > 1) ? atoi(argv[1]) : 2;
    switch (z) {
        case x: return x;
        case y: return y;
        default: return -1;
    }
    return x;
}
