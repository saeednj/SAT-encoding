#include "ppa.h"
#include <stdio.h>

ParallelPrefixAdder::ParallelPrefixAdder(int n)
    : Formula("")
{
    N = n;
}

ParallelPrefixAdder::~ParallelPrefixAdder()
{
}

void ParallelPrefixAdder::encode()
{
    int m =
        N == 2 ? 1 :
        N == 4 ? 2 :
        N == 8 ? 3 :
        N == 16 ? 4 :
        N == 32 ? 5 : -1;
    if ( m == -1 ) { printf("For now only considering bitwidth of power of two up to 32\n"); exit(1); }
    // TODO 1. Support a generic bit-width
    //      2. Some p variables are not used (last layer and first column)
    //      3. Add support for carry-in and carry-out

    int g[m+1][N], p[m+1][N];

    square(g[0], p[0], X, Y, N);
    int w = 1;
    for( int i=1; i<=m; i++ )
    {
        wcircle(g[i], p[i], g[i-1], p[i-1], w);
        bcircle(g[i]+w, p[i]+w, g[i-1]+w, p[i-1]+w, g[i-1], p[i-1], N-w);
        w *= 2;
    }
    xor2(Z, X, Y, 1);
    xor3(Z+1, X+1, Y+1, g[m], N-1);
}


void ParallelPrefixAdder::square(int *g, int *p, int *a, int *b, int n)
{
    newVars(g, n);
    newVars(p, n);
    and2(g, a, b, n);
    or2(p, a, b, n);
}

// generate:  g = g_left + g_right . p_left
// propagate: p = p_left . p_right
void ParallelPrefixAdder::bcircle(int *g, int *p, int *gl, int *pl, int *gr, int *pr, int n)
{
    newVars(g, n);
    newVars(p, n);
    for( int i=0; i<n; i++ )
    {
        addClause({-g[i], gl[i], gr[i]});
        addClause({-g[i], gl[i], pl[i]});
        addClause({g[i], -gl[i]});
        addClause({g[i], -gr[i], -pl[i]});
    }
    and2(p, pl, pr, n);
}

void ParallelPrefixAdder::wcircle(int *gout, int *pout, int *gin, int *pin, int n)
{
    assign(gout, gin, n);
    assign(pout, pin, n);
}


