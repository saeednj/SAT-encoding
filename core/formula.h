#ifndef _FORMULA_H_
#define _FORMULA_H_

#include <vector>
#include <string>
#include <map>
#include <queue>

using namespace std;

struct Clause
{
    vector<int> lits;
    bool xor_clause;
    Clause(vector<int> v, bool xorC = false)
    {
        xor_clause = xorC;
        lits = v;
    }
    Clause()
    {
        xor_clause = false;
    }
};

class Formula
{
    public:
        Formula();
        virtual ~Formula();

        void newVars(int *x, int n = 32, string name = "");

        void addClause(vector<int> v) { clauses.push_back(Clause(v)); }
        void addClause(Clause c) { clauses.push_back(c); }

        void fixedValue(int *z, unsigned v, int n = 32);

        int getVarCnt() { return varCnt; }
        int getClauseCnt() { return clauses.size(); }

        void setVarID(int v) { varID = v; }
        void setUseXORClauses() { useXORClauses = true; }
        void setUseFACardinality() { useFACardinality = true; }
        void setUseTseitinAdders() { useTseitinAdders = true; }

        void dimacs(bool header = true);

        /* operations */
        void rotl(int *z, int *x, int p, int n = 32);
        void rotr(int *z, int *x, int p, int n = 32) { rotl(z, x, n-p, n); }
        void assign(int *z, int *x) { rotl(z, x, 0); }
//        void shr(int *z, int *x, int n);
        void and2(int *z, int *x, int *y, int n = 32);
        void or2(int *z, int *x, int *y, int n = 32);
        void eq(int *z, int *x, int n = 32);
        void neq(int *z, int *x, int n = 32);
        void xor2(int *z, int *x, int *y, int n = 32);
        void xor3(int *z, int *x, int *y, int *t, int n = 32);
        void xor4(int *z, int *a, int *b, int *c, int *d, int n = 32);
        void ch(int *z, int *x, int *y, int *t, int n = 32);
        void maj3(int *z, int *x, int *y, int *t, int n = 32);
        void add2(int *z, int *x, int *y, int n = 32);
        void add3(int *z, int *a, int *b, int *c, int n = 32);
        void add4(int *z, int *a, int *b, int *c, int *d, int n = 32);
        void add5(int *z, int *a, int *b, int *c, int *d, int *e, int n = 32);


        void cardinality(int *vars, int n, unsigned cardinalValue);

        int clauseCheck();

        vector<Clause> getClauses();
        void AddFormula(Formula& f);

    protected:
        int varCnt, varID;
        bool useXORClauses;
        bool useFACardinality;
        bool useTseitinAdders;
        map<string, unsigned int> varNames;
        vector<Clause> clauses;

        void adder(const vector<int> &lhs, const vector<int> &rhs);

        int oneDeep(vector<queue<int>> &m);
    private:
};


#endif
