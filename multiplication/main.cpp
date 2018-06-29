#include "long.h"

int main()
{
    LongMult f(5);

//    int z[10] = {1, 0, 1, 1, 1, 1, 1, 1, 0, 0};
//    f.fixOutput(z, 10);

    f.fixOutput("253");
    f.cnf.dimacs();

    return 0;
}
