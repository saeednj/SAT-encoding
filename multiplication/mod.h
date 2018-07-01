#ifndef _MODULAR_MULT_H_
#define _MODULAR_MULT_H_

#define MAX_WIDTH 1024

#include "formula.h"
#include <string>
using namespace std;

class ModMult {
    public:
        /** the mod is in the form of "2^n - delta"
         *  delta values of {-1, 0, 1} are supported */
        ModMult(
                int n = 16,
                int delta = 1,
                bool useXorClauses = false);
        ~ModMult();

        void encode();
        void fixOutput(int *target, int len);
        void fixOutput(string target);

        int X[MAX_WIDTH];
        int Y[MAX_WIDTH];
        int Z[MAX_WIDTH];
        int N, Delta;

        Formula cnf;
};

#endif
