#include "hash.h"

MDHash::MDHash(int inSize, int outSize, int rnds, bool initBlock) :
    inputSize(inSize),
    outputSize(outSize),
    rounds(rnds),
    initialBlock(initBlock)
{
    w = new Word[rnds];
    chain = new Word[outSize];
    out = new Word[outSize];
}

MDHash::~MDHash()
{
    delete [] w;
    delete [] chain;
    delete [] out;
}

void MDHash::encode()
{
    throw "Base class encode!";
}

void MDHash::fixOutput(unsigned *target)
{
    for(int i=0; i<outputSize; i++)
        cnf.fixedValue(out[i], target[i]);
}

