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

#ifndef ATYPE
#define ATYPE TWO_OPERAND
#endif

    F.setMultiAdderType(Formula::ATYPE);

    int a[N], b[N], c[N], d[N], e[N], z[N];
    F.newVars(z, N);
    F.newVars(a, N);
    F.newVars(b, N);
    F.newVars(c, N);
    F.newVars(d, N);
    F.newVars(e, N);
    F.add5(z, a, b, c, d, e, N);

    unsigned _a = atoi(argv[1]);
    unsigned _b = atoi(argv[2]);
    unsigned _c = atoi(argv[3]);
    unsigned _d = atoi(argv[4]);
    unsigned _e = atoi(argv[5]);
    F.fixedValue(a, _a, N);
    F.fixedValue(b, _b, N);
    F.fixedValue(c, _c, N);
    F.fixedValue(d, _d, N);
    F.fixedValue(e, _e, N);

    F.dimacs();
    return 0;
}

