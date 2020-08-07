#ifndef _HASH_H_
#define _HASH_H_

#include "formula.h"

typedef int Word[32];

/** Interface class for Merkle-Damagard type hash functions
 */
class MDHash {
    public:
        MDHash(
                int inSize,
                int outSize,
                int rnds,
                bool initBlock = true);
        virtual ~MDHash();

        virtual void encode();
        void fixOutput(unsigned *target);

        int rounds;        /// Number of rounds
        bool initialBlock; /// Whether this is the first MD block

        int inputSize;  /// Number of message words
        int outputSize; /// Number of output words

        Word *w;    /// Message words
        Word *chain; /// Input chaining value (will be IV for initial block)
        Word *out;   /// Output chaining value

        Formula cnf;
};

#endif
