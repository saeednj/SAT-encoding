#ifndef _PARALLEL_PREFIX_H_
#define _PARALLEL_PREFIX_H_

#define MAX_WIDTH 1024

#include "formula.h"

class ParallelPrefixAdder : public Formula {
    public:
        ParallelPrefixAdder(int n = 32);
        ~ParallelPrefixAdder();

        void encode();

        int X[MAX_WIDTH];
        int Y[MAX_WIDTH];
        int Z[MAX_WIDTH];
        int N;

    private:
        void square(int *g, int *p, int *a, int *b, int n = 32);
        void bcircle(int *g, int *p, int *gl, int *pl, int *gr, int *pr, int n = 32);
        void wcircle(int *gout, int *pout, int *gin, int *pin, int n = 32);
};


#endif
