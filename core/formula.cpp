#include "formula.h"
#include <unistd.h>
#include <math.h>
#include <assert.h>
#include <string.h>
#include <sys/wait.h>

Formula::Formula(string name)
{
    varID = 0;
    varCnt = 0;
    useXORClauses = false;
    useFACardinality = false;
    adderType = RIPPLE_CARRY;
    multiAdderType = ESPRESSO;
    formulaName = name;
}

Formula::~Formula()
{
}

void Formula::newVars(int *x, int n, string name)
{
	for ( int i=0; i<n; i++ )
		x[i] = ++varID;

    if ( name != "" ) varNames[name] = x[0];
    varCnt += n;
}

void Formula::addClause(vector<int> v)
{
    if ( any_of(v.begin(), v.end(), [](int x){ return x==0; }) )
    {
        fprintf(stderr, "bad vector clause:");
        for( int x : v ) fprintf(stderr, " %d", x);
        fprintf(stderr, "\n");
        exit(1);
    }
    clauses.push_back(Clause(v));
}

void Formula::addClause(Clause c)
{
    if ( any_of(c.lits.begin(), c.lits.end(), [](int x){ return x==0; }) )
    {
        fprintf(stderr, "bad clause:");
        for( int x : c.lits ) fprintf(stderr, " %d", x);
        fprintf(stderr, "\n");
        exit(1);
    }
    clauses.push_back(c);
}

void Formula::fixedValue(int *z, unsigned value, int n)
{
    for( int i=0; i<n; i++ )
    {
        int x = (value >> i) & 1 ? z[i] : -z[i];
        addClause({x});
    }
}

void Formula::rotl(int *z, int *x, int p, int n)
{
	for( int i=0; i<n; i++ )
		z[i] = x[(i + n - p) % n];
}

/*void Formula::shr(int *z, int *x, int n)
{
	for( int i=n; i<32; i++ )
		z[i] = x[i-n];
}*/

void Formula::and2(int *z, int *x, int *y, int n)
{
    for( int i=0; i<n; i++ )
    {
        addClause( {z[i], -x[i], -y[i]} );
        addClause( {-z[i], x[i]} );
        addClause( {-z[i], y[i]} );
	}
}

void Formula::or2(int *z, int *x, int *y, int n)
{
    for( int i=0; i<n; i++ )
    {
        addClause( {-z[i], x[i], y[i]} );
        addClause( {z[i], -x[i]} );
        addClause( {z[i], -y[i]} );
	}
}

void Formula::eq(int *z, int *x, int n)
{
    for( int i=0; i<n; i++ )
    {
        addClause( {-z[i], x[i]} );
        addClause( {z[i], -x[i]} );
    }
}

void Formula::neq(int *z, int *x, int n)
{
    for( int i=0; i<n; i++ )
    {
        addClause( {z[i], x[i]} );
        addClause( {-z[i], -x[i]} );
    }
}

void Formula::xor2(int *z, int *x, int *y, int n)
{
    for( int i=0; i<n; i++ )
    {
        if ( useXORClauses )
        {
            addClause( Clause({-z[i], x[i], y[i]}, true) );
        }
        else
        {
            addClause( {-z[i], -x[i], -y[i]} );
            addClause( { z[i], -x[i],  y[i]} );
            addClause( { z[i],  x[i], -y[i]} );
            addClause( {-z[i],  x[i],  y[i]} );
        }
    }
}

void Formula::xor3(int *z, int *x, int *y, int *t, int n)
{
    for( int i=0; i<n; i++ )
    {
        if ( useXORClauses )
        {
            addClause( Clause({-z[i], x[i], y[i], t[i]}, true) );
        }
        else
        {
            addClause( { z[i], -x[i], -y[i], -t[i]} );
            addClause( {-z[i], -x[i], -y[i],  t[i]} );
            addClause( {-z[i], -x[i],  y[i], -t[i]} );
            addClause( { z[i], -x[i],  y[i],  t[i]} );
            addClause( {-z[i],  x[i], -y[i], -t[i]} );
            addClause( { z[i],  x[i], -y[i],  t[i]} );
            addClause( { z[i],  x[i],  y[i], -t[i]} );
            addClause( {-z[i],  x[i],  y[i],  t[i]} );
        }
    }
}

void Formula::xor4(int *z, int *a, int *b, int *c, int *d, int n)
{
    for( int i=0; i<n; i++ )
    {
        if ( useXORClauses )
        {
            addClause( Clause({-z[i], a[i], b[i], c[i], d[i]}, true) );
        }
        else
        {
            addClause( {-z[i], -a[i], -b[i], -c[i], -d[i]} );
            addClause( { z[i], -a[i], -b[i], -c[i],  d[i]} );
            addClause( { z[i], -a[i], -b[i],  c[i], -d[i]} );
            addClause( {-z[i], -a[i], -b[i],  c[i],  d[i]} );
            addClause( { z[i], -a[i],  b[i], -c[i], -d[i]} );
            addClause( {-z[i], -a[i],  b[i], -c[i],  d[i]} );
            addClause( {-z[i], -a[i],  b[i],  c[i], -d[i]} );
            addClause( { z[i], -a[i],  b[i],  c[i],  d[i]} );
            addClause( { z[i],  a[i], -b[i], -c[i], -d[i]} );
            addClause( {-z[i],  a[i], -b[i], -c[i],  d[i]} );
            addClause( {-z[i],  a[i], -b[i],  c[i], -d[i]} );
            addClause( { z[i],  a[i], -b[i],  c[i],  d[i]} );
            addClause( {-z[i],  a[i],  b[i], -c[i], -d[i]} );
            addClause( { z[i],  a[i],  b[i], -c[i],  d[i]} );
            addClause( { z[i],  a[i],  b[i],  c[i], -d[i]} );
            addClause( {-z[i],  a[i],  b[i],  c[i],  d[i]} );
        }
    }
}

void Formula::ch(int *z, int *x, int *y, int *t, int n)
{
    for( int i=0; i<n; i++ )
    {
        addClause( {-z[i], x[i], t[i]} );
        addClause( {-z[i], -x[i], y[i]} );
        addClause( {z[i], x[i], -t[i]} );
        addClause( {z[i], -x[i], -y[i]} );
    }
}

void Formula::maj3(int *z, int *x, int *y, int *t, int n)
{
    for( int i=0; i<n; i++ )
    {
        addClause( {-z[i], x[i], y[i]} );
        addClause( {-z[i], x[i], t[i]} );
        addClause( {-z[i], y[i], t[i]} );
        addClause( {z[i], -y[i], -t[i]} );
        addClause( {z[i], -x[i], -t[i]} );
        addClause( {z[i], -x[i], -y[i]} );
    }
}

void Formula::halfadder(int *c, int *s, int *x, int *y, int n)
{
    xor2(s, x, y, n);
    and2(c, x, y, n);
}

void Formula::fulladder(int *c, int *s, int *x, int *y, int *t, int n)
{
    xor3(s, x, y, t, n);
    maj3(c, x, y, t, n);
}

void Formula::counter(int *z, int *x, int n)
{
    assert( n <= 7 );

    int t[2], c[3];
    if ( n == 2 )
    {
        halfadder(z+1, z, x, x+1, 1);
    }
    else if ( n == 3 )
    {
        fulladder(z+1, z, x, x+1, x+2, 1);
    }
    else if ( n == 4 )
    {
        newVars(t, 1);
        newVars(c, 2);
        fulladder(c, t, x, x+1, x+2, 1);
        halfadder(c+1, z, x+3, t, 1);
        halfadder(z+2, z+1, c+1, c, 1);
    }
    else if ( n == 5 )
    {
        newVars(t, 1);
        newVars(c, 2);
        fulladder(c, t, x, x+1, x+2, 1);
        fulladder(c+1, z, t, x+3, x+4, 1);
        halfadder(z+2, z+1, c+1, c, 1);
    }
    else if ( n == 6 )
    {
        newVars(t, 2);
        newVars(c, 3);
        fulladder(c, t, x, x+1, x+2, 1);
        fulladder(c+1, t+1, x+3, x+4, x+5, 1);
        halfadder(c+2, z, t, t+1, 1);
        fulladder(z+2, z+1, c+2, c+1, c, 1);
    }
    else if ( n == 7 )
    {
        newVars(t, 2);
        newVars(c, 3);
        fulladder(c, t, x, x+1, x+2, 1);
        fulladder(c+1, t+1, x+3, x+4, x+5, 1);
        fulladder(c+2, z, t, t+1, x+6, 1);
        fulladder(z+2, z+1, c+2, c+1, c, 1);
    }
}

void Formula::add2(int *z, int *x, int *y, int n)
{
    if ( multiAdderType == TWO_OPERAND || multiAdderType == DOT_MATRIX )
    {
        int c[n-1];
        newVars(c, n-1);

        // xor2(z, x, y, 1);
        // and2(c, x, y, 1);
        halfadder(c, z, x, y, 1);

        // xor3(z+1, x+1, y+1, c, n-1);
        // maj3(c+1, x+1, y+1, c, n-2);
        fulladder(c+1, z+1, x+1, y+1, c, n-2);
        xor3(z+n-1, x+n-1, y+n-1, c+n-2, 1);
    }
    else
    {
        vector<int> addends[n + 5];
        for( int i=0; i<n; i++ )
        {
            addends[i].push_back(x[i]);
            addends[i].push_back(y[i]);

            unsigned int m = floor(log2(addends[i].size()));
            std::vector<int> sum(1 + m);
            sum[0] = z[i];
            newVars(&sum[1], m);

            for( int j=1; j<1+m; j++ )
                addends[i + j].push_back(sum[j]);

            if ( multiAdderType == COUNTER_CHAIN )
                counter(&sum[0], &addends[i][0], addends[i].size());
            else if ( multiAdderType == ESPRESSO )
                espresso(addends[i], sum);
            else
            {
                printf("INVALID MULTI OPERAND ADDER TYPE in add2!\n");
                exit(1);
            }

        }
    }
}

void Formula::add3(int *z, int *a, int *b, int *c, int n)
{
    if ( multiAdderType == TWO_OPERAND )
    {
        int t0[n];
        newVars(t0, n);
        add2(t0, a, b, n);

        int t1[n];
        newVars(t1, n);
        add2(z, c, t0, n);
    }
    else if ( multiAdderType == DOT_MATRIX )
    {
        int t0[n-1];
        int c0[n-1];
        newVars(t0, n-1);
        newVars(c0, n-1);
        xor3(z, a, b, c, 1);
        xor3(t0, a+1, b+1, c+1, n-1);
        maj3(c0, a, b, c, n-1);

        add2(z+1, t0, c0, n-1);
    }
    else
    {
        vector<int> addends[n + 5];
        for( int i=0; i<n; i++ )
        {
            addends[i].push_back(a[i]);
            addends[i].push_back(b[i]);
            addends[i].push_back(c[i]);

            unsigned int m = floor(log2(addends[i].size()));
            std::vector<int> sum(1 + m);
            sum[0] = z[i];
            newVars(&sum[1], m);

            for( int j=1; j<1+m; j++ )
                addends[i + j].push_back(sum[j]);

            if ( multiAdderType == COUNTER_CHAIN )
                counter(&sum[0], &addends[i][0], addends[i].size());
            else if ( multiAdderType == ESPRESSO )
                espresso(addends[i], sum);
            else
            {
                printf("INVALID MULTI OPERAND ADDER TYPE in add3!\n");
                exit(1);
            }
        }
    }
}

void Formula::add4(int *z, int *a, int *b, int *c, int *d, int n)
{
    if ( multiAdderType == TWO_OPERAND )
    {
        int t0[n];
        newVars(t0, n);
        add2(t0, a, b, n);

        int t1[n];
        newVars(t1, n);
        add2(t1, c, d, n);

        add2(z, t0, t1, n);
    }
    else if ( multiAdderType == DOT_MATRIX )
    {
        int t0[n];
        int c0[n-1];
        newVars(t0, n);
        newVars(c0, n-1);
        xor3(t0, a, b, c, n);
        maj3(c0, a, b, c, n-1);

        int t1[n-1];
        int c1[n-1];
        newVars(t1, n-1);
        newVars(c1, n-1);
        xor2(z, t0, d, 1);
        and2(c1, t0, d, 1);
        xor3(t1, t0+1, c0, d+1, n-1);
        maj3(c1+1, t0+1, c0, d+1, n-2);

        add2(z+1, t1, c1, n-1);

    }
    else
    {
        vector<int> addends[n + 5];
        for( int i=0; i<n; i++ )
        {
            addends[i].push_back(a[i]);
            addends[i].push_back(b[i]);
            addends[i].push_back(c[i]);
            addends[i].push_back(d[i]);

            unsigned int m = floor(log2(addends[i].size()));
            std::vector<int> sum(1 + m);
            sum[0] = z[i];
            newVars(&sum[1], m);

            for( int j=1; j<1+m; j++ )
                addends[i + j].push_back(sum[j]);

            if ( multiAdderType == COUNTER_CHAIN )
                counter(&sum[0], &addends[i][0], addends[i].size());
            else if ( multiAdderType == ESPRESSO )
                espresso(addends[i], sum);
            else
            {
                printf("INVALID MULTI OPERAND ADDER TYPE in add4!\n");
                exit(1);
            }
        }
    }
}

void Formula::add5(int *z, int *a, int *b, int *c, int *d, int *e, int n)
{
    if ( multiAdderType == TWO_OPERAND )
    {
        int t0[n];
        newVars(t0, n);
        add2(t0, a, b, n);

        int t1[n];
        newVars(t1, n);
        add2(t1, c, d, n);

        int t2[n];
        newVars(t2, n);
        add2(t2, t0, t1, n);

        add2(z, t2, e, n);
    }
    else if ( multiAdderType == DOT_MATRIX )
    {
        int t0[n];
        int c0[n-1];
        newVars(t0, n);
        newVars(c0, n-1);
        xor3(t0, a, b, c, n);
        maj3(c0, a, b, c, n-1);


        int t1[n-1];
        int c1[n-1];
        newVars(t1, n-1);
        newVars(c1, n-1);
        xor3(z, t0, d, e, 1);
        maj3(c1, t0, d, e, 1);
        xor3(t1, t0+1, c0, d+1, n-1);
        maj3(c1+1, t0+1, c0, d+1, n-2);

        int t2[n-2];
        int c2[n-2];
        newVars(t2, n-2);
        newVars(c2, n-2);
        xor3(z+1, t1, c1, e+1, 1);
        maj3(c2, t1, c1, e+1, 1);
        xor3(t2, t1+1, c1+1, e+2, n-2);
        maj3(c2+1, t1+1, c1+1, e+2, n-3);

        add2(z+2, t2, c2, n-2);
    }
    else
    {
        vector<int> addends[n + 5];
        for( int i=0; i<n; i++ )
        {
            addends[i].push_back(a[i]);
            addends[i].push_back(b[i]);
            addends[i].push_back(c[i]);
            addends[i].push_back(d[i]);
            addends[i].push_back(e[i]);

            unsigned int m = floor(log2(addends[i].size()));
            std::vector<int> sum(1 + m);
            sum[0] = z[i];
            newVars(&sum[1], m);

            for( int j=1; j<1+m; j++ )
                addends[i + j].push_back(sum[j]);

            if ( multiAdderType == COUNTER_CHAIN )
                counter(&sum[0], &addends[i][0], addends[i].size());
            else if ( multiAdderType == ESPRESSO )
                espresso(addends[i], sum);
            else
            {
                printf("INVALID MULTI OPERAND ADDER TYPE in add5!\n");
                exit(1);
            }
        }
    }
}

void Formula::espresso(const vector<int> &lhs, const vector<int> &rhs)
{
    static map<pair<unsigned int, unsigned int>, vector<vector<int>>> cache;

    unsigned int n = lhs.size();
    unsigned int m = rhs.size();

    vector<vector<int>> _clauses;
    auto it = cache.find(make_pair(n, m));
    if (it != cache.end())
    {
        _clauses = it->second;
    }
    else
    {
        int wfd[2], rfd[2];

        /* pipe(): fd[0] is for reading, fd[1] is for writing */

        if (pipe(wfd) == -1)
            throw std::runtime_error("pipe() failed");

        if (pipe(rfd) == -1)
            throw std::runtime_error("pipe() failed");

        pid_t child = fork();
        if (child == 0)
        {
            if (dup2(wfd[0], STDIN_FILENO) == -1)
                throw std::runtime_error("dup() failed");

            if (dup2(rfd[1], STDOUT_FILENO) == -1)
                throw std::runtime_error("dup() failed");

            if (execlp("espresso", "espresso", 0) == -1)
                throw std::runtime_error("execve() failed");

            exit(EXIT_FAILURE);
        }

        close(wfd[0]);
        close(rfd[1]);

        FILE *eout = fdopen(wfd[1], "w");
        if (!eout)
            throw std::runtime_error("fdopen() failed");

        FILE *ein = fdopen(rfd[0], "r");
        if (!ein)
            throw std::runtime_error("fdopen() failed");

        fprintf(eout, ".i %u\n", n + m);
        fprintf(eout, ".o 1\n");

        for (unsigned int i = 0; i < 1U << n; ++i)
        {
            for (unsigned int j = 0; j < 1U << m; ++j)
            {
                for (unsigned int k = n; k--; )
                    fprintf(eout, "%u", 1 - ((i >> k) & 1));
                for (unsigned int k = m; k--; )
                    fprintf(eout, "%u", 1 - ((j >> k) & 1));

                fprintf(eout, " %u\n", __builtin_popcount(i) != j);
            }
        }

        fprintf(eout, ".e\n");
        fflush(eout);

        while (1)
        {
            char buf[512];
            if (!fgets(buf, sizeof(buf), ein))
                break;

            if (!strncmp(buf, ".i", 2))
                continue;
            if (!strncmp(buf, ".o", 2))
                continue;
            if (!strncmp(buf, ".p", 2))
                continue;
            if (!strncmp(buf, ".e", 2))
                break;

            vector<int> c;
            for (int i = 0; i < n + m; ++i)
            {
                if (buf[i] == '0')
                    c.push_back(-(i + 1));
                else if (buf[i] == '1')
                    c.push_back(i + 1);
            }

            _clauses.push_back(c);
        }

        fclose(ein);
        fclose(eout);

        while (true)
        {
            int status;
            pid_t kid = wait(&status);
            if (kid == -1)
            {
                if (errno == ECHILD)
                    break;
                if (errno == EINTR)
                    continue;

                throw std::runtime_error("wait() failed");
            }

            if (kid == child)
                break;
        }

        cache.insert(make_pair(make_pair(n, m), _clauses));

#ifdef _DUMP_ADDER_CLAUSES_
        FILE *f = fopen("comp_clauses.txt", "a");
        fprintf(f, "%d %d %d\n", n, m, _clauses.size());
        for( vector<int> &c: _clauses )
        {
            for( int i: c )
                fprintf(f, "%d ", i);
            fprintf(f, "\n");
        }
        fclose(f);
#endif
    }

    for(vector<int> &c: _clauses)
    {
        Clause cl;
        for (int i: c)
        {
            int j = abs(i) - 1;
            int var = j < n ? lhs[j] : rhs[m - 1 - (j - n)];
            if ( i < 0 )
                cl.lits.push_back(-var);
            else
                cl.lits.push_back(var);
        }

        addClause(cl);
    }
}


void Formula::dimacs(bool header)
{
    if ( header )
        printf("p cnf %d %d\n", getVarCnt(), getClauseCnt());

    for( Clause c : clauses )
    {
        if ( c.xor_clause ) printf("x ");
        for( int v : c.lits )
            printf("%d ", v);
        printf("0\n");
    }

    for( auto e : varNames )
        printf("c %s %d\n", e.first.c_str(), e.second);
}

int Formula::clauseCheck()
{
    for( Clause c : clauses )
    {
        for( int v : c.lits )
        {
            if ( abs(v) > getVarCnt() ) { fprintf(stderr, "Clause check failed: out of bound variable ID (%d)! var_cnt == %d\n", v, getVarCnt()); abort(); }
            if ( v == 0 ) { fprintf(stderr, "Clause check failed: variable ID is zero!\n"); abort(); }
        }

    }
    return 0;
}


void Formula::cardinality(int *vars, int n, unsigned cardinalValue)
{
    unsigned int size = 1 + floor(log2(n));
    vector<queue<int>> m(size);
    for( int i=0; i<n; i++ )
        m[0].push(vars[i]);

    bool oneDeep = false;
    if ( useFACardinality )
    {
        while( !oneDeep )
        {
            oneDeep = true;
            for( int i=0; i<m.size(); i++ )
            {
                if ( m[i].size() >= 3 )
                {
                    int x = m[i].front(); m[i].pop();
                    int y = m[i].front(); m[i].pop();
                    int z = m[i].front(); m[i].pop();

                    int sum;
                    newVars(&sum, 1);
                    xor3(&sum, &x, &y, &z, 1);
                    m[i].push(sum);

                    if ( i+1 < m.size() )
                    {
                        int carry;
                        newVars(&carry, 1);
                        maj3(&carry, &x, &y, &z, 1);
                        m[i+1].push(carry);
                    }
                }
                else if ( m[i].size() >= 2 )
                {
                    int x = m[i].front(); m[i].pop();
                    int y = m[i].front(); m[i].pop();

                    int sum;
                    newVars(&sum, 1);
                    xor2(&sum, &x, &y, 1);
                    m[i].push(sum);

                    if ( i+1 < m.size() )
                    {
                        int carry;
                        newVars(&carry, 1);
                        and2(&carry, &x, &y, 1);
                        m[i+1].push(carry);
                    }
                }

                if ( m[i].size() > 1 ) oneDeep = false;
            }
        }

    }
    else
    {
        while( !oneDeep )
        {
            oneDeep = true;
            for( int i=0; i<m.size(); i++ )
            {
                if ( m[i].size() >= 2 )
                {
                    int inpSize = m[i].size() > 10 ? 10 : m[i].size();
                    vector<int> addends;
                    for( int j=0; j<inpSize; j++ )
                    {
                        int x = m[i].front();
                        m[i].pop();
                        addends.push_back(x);
                    }
                    unsigned int slen = floor(log2(addends.size()));
                    vector<int> sum(slen + 1);
                    newVars(&sum[0], slen+1);
                    espresso(addends, sum);

                    for( int j=0; j<slen+1 && i+j<m.size(); j++ )
                        m[i+j].push(sum[j]);
                }

                if ( m[i].size() > 1 ) oneDeep = false;
            }
        }
    }

    for( int i=0; i<m.size(); i++ )
    {
        int var = m[i].front();
        unsigned val = (cardinalValue >> i) & 1;
        fixedValue(&var, val, 1);
    }
}

vector<Clause> Formula::getClauses()
{
    return clauses;
}

void Formula::AddFormula(Formula& f)
{
    varCnt += f.getVarCnt();
    vector<Clause> c = f.getClauses();
    clauses.insert(clauses.end(), c.begin(), c.end());
    for( auto e : f.varNames )
        varNames[e.first + "_" + f.formulaName] = e.second;
}
