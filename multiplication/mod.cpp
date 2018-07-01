#include "mod.h"

ModMult::ModMult(int n, int delta, bool useXorClauses)
{
    N = n;
    Delta = delta;
    if ( useXorClauses ) cnf.setUseXORClauses();

    encode();
}


ModMult::~ModMult()
{
}


void ModMult::encode()
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

    if ( Delta == 1 )
    {
        vector<queue<int>> m(N);

        for( int i=0; i<N; i++ )
            for( int j=0; j<N; j++ )
                m[(j+i)%N].push(table[i][j]);

        /* Compress the dots */
        bool twoDeep = false;

        while( !twoDeep )
        {
            twoDeep = true;

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

                    int carry;
                    cnf.newVars(&carry, 1);
                    cnf.maj3(&carry, &x, &y, &z, 1);
                    m[(i+1)%m.size()].push(carry);
                }

                if ( m[i].size() > 2 ) twoDeep = false;
            }
        }

        /* Carry-around ripple carry adder */
        int x[N], y[N], c[N];
        for( int i=0; i<N; i++ )
        {
            x[i] = m[i].front();
            m[i].pop();
            y[i] = m[i].front();
            m[i].pop();
        }
        cnf.newVars(c, N);
        cnf.newVars(Z, N);

        cnf.xor3(Z, x, y, c, N);
        cnf.maj3(c+1, x, y, c, N-1);
        cnf.maj3(c, x+N-1, y+N-1, c+N-1, 1);
    }
    else
    {
        printf("NOT IMPLEMENTED YET\n");
    }
}

void ModMult::fixOutput(int *target, int len)
{
    for( int i=0; i<len; i++ )
        cnf.fixedValue(&Z[i], target[i], 1);
}

void ModMult::fixOutput(string target)
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

