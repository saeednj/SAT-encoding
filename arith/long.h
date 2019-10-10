#ifndef _LONG_MULT_H_
#define _LONG_MULT_H_

#define MAX_WIDTH 1024

#include "formula.h"
#include <string>
using namespace std;

class LongMult {
    public:
        LongMult(
                int n = 32,
                bool useXorClauses = false);
        ~LongMult();

        void encode();
        void fixOutput(int *target, int len);
        void fixOutput(string target);

        int X[MAX_WIDTH];
        int Y[MAX_WIDTH];
        int Z[2*MAX_WIDTH];
        int N;

        Formula cnf;
};

#endif
