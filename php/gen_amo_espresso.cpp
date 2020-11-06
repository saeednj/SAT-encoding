#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "formula.h"

int main(int argc, char** argv)
{
    int n = atoi(argv[1]);
    Formula f;
    f.setUseFACardinality();

    int v[n][n-1];
    for( int i=0; i<n; i++ )
        f.newVars(v[i], n-1);

    for( int i=0; i<n; i++ )
    {
        vector<int> t(v[i], v[i]+n-1);
        f.addClause(t);
    }

    int x[n];
    for( int j=0; j<n-1; j++ )
    {
        for( int i=0; i<n; i++ )
            x[i] = v[i][j];
        f.cardinality(x, n, 1);
    }
    f.dimacs();

    return 0;
}

