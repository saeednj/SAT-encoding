#include "sha1.h"
#include "formula.h"

SHA1::SHA1(int rnds, bool initBlock)
{
    rounds = rnds;
    initialBlock = initBlock;
}

void SHA1::encode()
{
    for( int i=0; i<16; i++ )
        cnf.newVars(w[i], 32, "w"+to_string(i));

    int wt[80][32];
    for( int i=16; i<rounds; i++ )
        cnf.newVars(wt[i], 32, "w"+to_string(i));

    for( int i=0; i<5; i++ )
        cnf.newVars(in[i], 32);

    for( int i=0; i<5; i++ )
        cnf.newVars(out[i], 32, "hash"+to_string(i));

    for( int i=0; i<rounds; i++ )
        cnf.newVars(a[i + 5], 32);

    /* Message expansion */
    for( int i=16; i<rounds; i++ )
    {
        cnf.xor4(wt[i], w[i-3], w[i-8], w[i-14], w[i-16]);
        cnf.rotl(w[i], wt[i], 1);
    }

    /* Round constants */
    int k[4][32];
    for( int i=0; i<4; i++ )
        cnf.newVars(k[i]);
    cnf.fixedValue(k[0], 0x5a827999);
    cnf.fixedValue(k[1], 0x6ed9eba1);
    cnf.fixedValue(k[2], 0x8f1bbcdc);
    cnf.fixedValue(k[3], 0xca62c1d6);

    /* Initialization vector */
    if ( initialBlock )
    {
        cnf.fixedValue(in[0], 0x67452301);
        cnf.fixedValue(in[1], 0xefcdab89);
        cnf.fixedValue(in[2], 0x98badcfe);
        cnf.fixedValue(in[3], 0x10325476);
        cnf.fixedValue(in[4], 0xc3d2e1f0);
    }

    cnf.rotl(a[4], in[0], 0);
    cnf.rotl(a[3], in[1], 0);
    cnf.rotl(a[2], in[2], 2);
    cnf.rotl(a[1], in[3], 2);
    cnf.rotl(a[0], in[4], 2);

    /* Main loop */
    for( int i=0; i<rounds; i++ )
    {
        int prev_a[32], b[32], c[32], d[32], e[32];
        cnf.rotl(prev_a, a[i+4], 5);
        cnf.rotl(b, a[i+3], 0);
        cnf.rotl(c, a[i+2], 30);
        cnf.rotl(d, a[i+1], 30);
        cnf.rotl(e, a[i], 30);

        int f[32];
        cnf.newVars(f, 32);

        if ( i < 20 )
        {
            cnf.ch(f, b, c, d);
        }
        else if ( i < 40 )
        {
            cnf.xor3(f, b, c, d);
        }
        else if ( i < 60 )
        {
            cnf.maj3(f, b, c, d);
        }
        else
        {
            cnf.xor3(f, b, c, d);
        }

        cnf.add5(a[i + 5], prev_a, f, e, k[i/20], w[i]);
    }

    /* Rotate back */
    int c[32];
    cnf.rotl(c, a[rounds+2], 30);

    int d[32];
    cnf.rotl(d, a[rounds+1], 30);

    int e[32];
    cnf.rotl(e, a[rounds], 30);

    /* Final addition */
    cnf.add2(out[0], in[0], a[rounds+4]);
    cnf.add2(out[1], in[1], a[rounds+3]);
    cnf.add2(out[2], in[2], c);
    cnf.add2(out[3], in[3], d);
    cnf.add2(out[4], in[4], e);
}

void SHA1::fixOutput(unsigned target[5])
{
    for( int i=0; i<5; i++ )
        cnf.fixedValue(out[i], target[i]);
}
