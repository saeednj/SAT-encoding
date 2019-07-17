#ifndef _SHA256_H_
#define _SHA256_H_

#include "formula.h"

class SHA256 {
    public:
        SHA256(
                int rnds = 64,
                bool initBlock = true);

        void encode();
        void fixOutput(unsigned target[8]);

        int w[64][32];
        int in[8][32];
        int out[8][32];

        int A[70][32];
        int E[70][32];

        int rounds;
        bool initialBlock;

        Formula cnf;

        void Sigma0(int *z, int *x);
        void Sigma1(int *z, int *x);
};

#endif
