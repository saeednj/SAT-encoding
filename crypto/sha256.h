#ifndef _SHA256_H_
#define _SHA256_H_

#include "hash.h"

class SHA256 : public MDHash {
    public:
        SHA256(int rnds = 64, bool initBlock = true);

        void encode();

        int A[70][32];
        int E[70][32];

        void Sigma0(int *z, int *x);
        void Sigma1(int *z, int *x);
};

#endif
