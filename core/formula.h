#ifndef _FORMULA_H_
#define _FORMULA_H_

#include <vector>
#include <string>
#include <map>
#include <queue>
#include <algorithm>

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
        Formula(string name = "");
        virtual ~Formula();

        void newVars(int *x, int n = 32, string name = "");                     // Reserves new variable IDs for the bitvector 'x' of size 'n'

        void addClause(vector<int> v);
        void addClause(Clause c);

        void fixedValue(int *z, unsigned v, int n = 32);                        // Forces the bitvector 'z' to the value 'v'

        int getVarCnt() { return varCnt; }
        int getClauseCnt() { return clauses.size(); }

        enum AdderType {
            AT_NONE,
            RIPPLE_CARRY,
        };

        enum MultiAdderType {
            MAT_NONE,
            TWO_OPERAND,              // Adding two operands at a time
            COUNTER_CHAIN,            // Using counters (adding up bits in a single column) in a ripple carry fashion
            ESPRESSO,                 // Similar to COUNTER_CHAIN, but uses espresso logic minimizer instead of half/full adders
            DOT_MATRIX,               // Reducing the whole dot matrix of operand bits using wallace tree (and half/full adders)
        };

        void setVarID(int v) { varID = v; }                                     // Sets the starting point of variable IDs
        void setUseXORClauses() { useXORClauses = true; }
        void setUseFACardinality() { useFACardinality = true; }
        void setAdderType(AdderType type) { adderType = type; }
        void setMultiAdderType(MultiAdderType type) { multiAdderType = type; }

        void dimacs(bool header = true);                                        // Prints the current clause database in DIMACS format to stdout

        /* operations */
        void rotl(int *z, int *x, int p, int n = 32);                           // Rotate left 'p' postitions
        void rotr(int *z, int *x, int p, int n = 32) { rotl(z, x, n-p, n); }    // Rotate right 'p' positions
        void assign(int *z, int *x, int n = 32) { rotl(z, x, 0, n); }
        void and2(int *z, int *x, int *y, int n = 32);                          // Two-input AND
        void or2(int *z, int *x, int *y, int n = 32);                           // Two-input OR
        void eq(int *z, int *x, int n = 32);                                    // Equivalence
        void neq(int *z, int *x, int n = 32);                                   // Boolean negation
        void xor2(int *z, int *x, int *y, int n = 32);                          // Two-input XOR
        void xor3(int *z, int *x, int *y, int *t, int n = 32);                  // Three-input XOR
        void xor4(int *z, int *a, int *b, int *c, int *d, int n = 32);          // Four-input XOR
        void ch(int *z, int *x, int *y, int *t, int n = 32);                    // 'IF' function (used in SHA round functions). z = x ? y : t;
        void maj3(int *z, int *x, int *y, int *t, int n = 32);                  // Three-input Majority function


        void halfadder(int *c, int *s, int *x, int *y, int n);
        void fulladder(int *c, int *s, int *x, int *y, int *t, int n);

        void add2(int *z, int *a, int *b, int n = 32);                          // z = a + b;
        void add3(int *z, int *a, int *b, int *c, int n = 32);                  // z = a + b + c;
        void add4(int *z, int *a, int *b, int *c, int *d, int n = 32);          // z = a + b + c + d;
        void add5(int *z, int *a, int *b, int *c, int *d, int *e, int n = 32);  // z = a + b + c + d + e;


        void cardinality(int *vars, int n, unsigned cardinalValue);             // Cardinality constraints: vars[0]+vars[1]+...+vars[n-1] == cardinalValue 

        int clauseCheck();                                                      // Mainly for debugging. Checks trivial invalid clauses 

        vector<Clause> getClauses();
        void AddFormula(Formula& f);

        map<string, unsigned int> varNames;                                     // labels for variable IDs
        string formulaName;

    protected:
        int varCnt, varID;
        bool useXORClauses;
        bool useFACardinality;
        AdderType adderType;
        MultiAdderType multiAdderType;
        vector<Clause> clauses;

        void espresso(const vector<int> &lhs, const vector<int> &rhs);             // deriving lhs = addition(rhs), through espresso minimization;

        void counter(int *z, int *x, int n);

    private:
};


#endif
