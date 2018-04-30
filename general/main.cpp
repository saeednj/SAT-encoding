#include "sha1.h"
#include "util.h"
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

/* options */
bool use_xor_clauses;
bool use_normal_adders;
bool use_rand_bit;


void preimage(int rounds)
{
	SHA1 f(rounds, true, use_xor_clauses, use_normal_adders);

    unsigned w[80];
    unsigned hash[5];

    if ( use_rand_bit )
    {
        for( int i=0; i<16; i++ )
            w[i] = lrand48();

        sha1_comp(w, hash, rounds);
    }
    else
    {
        /* Read the randomly generated input and its hash */
        for( int i=0; i<16; i++ )
            scanf("%x", &w[i]);

        for( int i=0; i<5; i++ )
            scanf("%x", &hash[i]);

        /* Double checking the message words with hash target */
        unsigned h[5];
        sha1_comp(w, h, rounds);
        assert( hash[0] == h[0] );
        assert( hash[1] == h[1] );
        assert( hash[2] == h[2] );
        assert( hash[3] == h[3] );
        assert( hash[4] == h[4] );
    }


	/* Set hash target bits */
    f.fixOutput(hash);

    /* Printing out the instance */
    f.cnf.dimacs();
}

int main(int argc, char **argv)
{
	unsigned long seed = time( NULL );

    /* Arguments default values */
    use_xor_clauses = false;
    use_normal_adders = false;
    use_rand_bit = false;
    int rounds = 80;

	/* Process command line */
    int c;
    while ((c = getopt (argc, argv, "axr:b")) != -1)
    {
        switch (c)
        {
            case 'a':
                use_normal_adders = true;
                break;
            case 'x':
                use_xor_clauses = true;
                break;
            case 'r':
                rounds = atoi(optarg);
                break;
            case 'b':
                use_rand_bit = true;
                break;
            case '?':
                if (optopt == 'r')
                    fprintf (stderr, "Please specify the number of rounds with -r.\n");
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                            "Unknown option character `\\x%x'.\n",
                            optopt);
                return 1;
            default:
                abort ();
        }
    }

	srand(seed);
	srand48(rand());

    preimage(rounds);


	return 0;
}
