#ifndef _SHA1_H_
#define _SHA1_H_

#include "hash.h"

class SHA1 : public MDHash {
    public:
        SHA1(int rnds = 80, bool initBlock = true);

        void encode();

        int a[85][32];
};

#endif
