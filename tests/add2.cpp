#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "formula.h"

#ifndef N
#define N 8
#endif

int main(int argc, char** argv)
{
    Formula F;

    int x[N], y[N], z[N];
    F.newVars(z, N);
    F.newVars(x, N);
    F.newVars(y, N);
    F.add2(z, x, y, N);

    unsigned _x = atoi(argv[1]);
    unsigned _y = atoi(argv[2]);
    F.fixedValue(x, _x, N);
    F.fixedValue(y, _y, N);

    F.dimacs();
    return 0;
}

