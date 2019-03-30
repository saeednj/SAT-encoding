#ifndef _SHA1_H_
#define _SHA1_H_

#include "formula.h"

class SHA1 {
    public:
        SHA1(int rnds = 80, bool initBlock = true);

        void encode();
        void fixOutput(unsigned target[5]);

        int w[80][32];
        int in[5][32];
        int out[5][32];
        int a[85][32];
        int rounds;
        bool initialBlock;

        Formula cnf;
};

#endif
