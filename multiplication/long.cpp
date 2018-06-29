#include "long.h"
#include <queue>
#include <vector>
#include <stdio.h>

LongMult::LongMult(int n, bool useXorClauses)
{
    N = n;
    if ( useXorClauses ) cnf.setUseXORClauses();

    encode();
}

LongMult::~LongMult()
{
}

void LongMult::encode()
{
    int table[MAX_WIDTH][MAX_WIDTH];

    cnf.newVars(X, N, "X");
    cnf.newVars(Y, N, "Y");
    for( int i=0; i<N; i++ )
        cnf.newVars(table[i], N);

    /* Generate the bit-by-bit multiplication table */ 
    for( int j=0; j<N; j++ )
        for( int i=0; i<N; i++ )
            cnf.and2(&table[j][i], &X[i], &Y[j], 1);

    vector<queue<int>> m(2*N);

    for( int i=0; i<N; i++ )
        for( int j=0; j<N; j++ )
            m[j+i].push(table[i][j]);

    /* Compress the dots */
    bool oneDeep = false;

    while( !oneDeep )
    {
        oneDeep = true;

        for( int i=0; i<m.size(); i++ )
        {
            /* Full adder */
            if ( m[i].size() >= 3 )
            {
                int x = m[i].front(); m[i].pop();
                int y = m[i].front(); m[i].pop();
                int z = m[i].front(); m[i].pop();

                int sum;
                cnf.newVars(&sum, 1);
                cnf.xor3(&sum, &x, &y, &z, 1);
                m[i].push(sum);

                if ( i+1 < m.size() )
                {
                    int carry;
                    cnf.newVars(&carry, 1);
                    cnf.maj3(&carry, &x, &y, &z, 1);
                    m[i+1].push(carry);
                }
            }
            /* Half adder */
            else if ( m[i].size() >= 2 )
            {
                int x = m[i].front(); m[i].pop();
                int y = m[i].front(); m[i].pop();

                int sum;
                cnf.newVars(&sum, 1);
                cnf.xor2(&sum, &x, &y, 1);
                m[i].push(sum);

                if ( i+1 < m.size() )
                {
                    int carry;
                    cnf.newVars(&carry, 1);
                    cnf.and2(&carry, &x, &y, 1);
                    m[i+1].push(carry);
                }
            }

            if ( m[i].size() > 1 ) oneDeep = false;
        }
    }

    /* Copy the compressed variables to the output */
    for( int i=0; i<m.size(); i++ )
        Z[i] = m[i].front();
}

void LongMult::fixOutput(int *target, int len)
{
    for( int i=0; i<len; i++ )
        cnf.fixedValue(&Z[i], target[i], 1);
}

void LongMult::fixOutput(string target)
{
    int x[2*MAX_WIDTH];
    int n = 0;

    while( target.size() > 0 )
    {
        int r = 0;
        for( int i=0; i<target.size(); i++ )
        {
            int d = r * 10 + target[i] - '0';
            r = d % 2;
            target[i] = d / 2 + '0';
        }
        x[n++] = r;
        while( target.size() > 0 && target[0] == '0' )
            target.erase(0, 1);
    }

    while( n < 2*N )
        x[n++] = 0;

    fixOutput(x, 2*N);

#ifdef DEBUG
    for( int i=n-1; i>=0; i-- )
    {
        printf("%d", x[i]);
        if ( i % 4 == 0 ) printf(" ");
    }
    printf("\n");
#endif
}


