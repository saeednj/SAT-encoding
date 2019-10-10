#include "long.h"
#include "mod.h"
#include "ppa.h"

#define TEST3

int main()
{
#ifdef TEST1
    LongMult f(5);

//    int z[10] = {1, 0, 1, 1, 1, 1, 1, 1, 0, 0};
//    f.fixOutput(z, 10);

    f.fixOutput("253");
    f.cnf.dimacs();
#endif

#ifdef TEST2
    ModMult f(3);
    int z[3] = {1, 0, 0};
    f.fixOutput(z, 3);
    f.cnf.dimacs();
#endif

#ifdef TEST3
    ParallelPrefixAdder ppa(8);

    ppa.newVars(ppa.X, 8);
    ppa.newVars(ppa.Y, 8);
    ppa.newVars(ppa.Z, 8);

    ppa.fixedValue(ppa.Z, 0xab, 8);

    ppa.encode();

    ppa.dimacs();
#endif
    return 0;
}
