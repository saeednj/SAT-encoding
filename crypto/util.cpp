#include "util.h"
#include <vector>
using namespace std;

unsigned rotl(unsigned x, int n)
{
	return (x << n) | (x >> (32 - n));
}

unsigned rotr(unsigned x, int n)
{
    return (x >> n) | (x << (32-n));
}

unsigned shr(unsigned x, int n)
{
    return (x >> n);
}

void sha1_comp(unsigned w[80], unsigned h[5], int rounds, bool chaining)
{
    if ( !chaining )
    {
        h[0] = 0x67452301;
        h[1] = 0xEFCDAB89;
        h[2] = 0x98BADCFE;
        h[3] = 0x10325476;
        h[4] = 0xC3D2E1F0;
    }

	for( int i=16; i<rounds; i++ )
		w[i] = rotl(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

	unsigned a = h[0];
	unsigned b = h[1];
	unsigned c = h[2];
	unsigned d = h[3];
	unsigned e = h[4];

    unsigned f, k;
	for( int i=0; i<rounds; i++ )
    {
		if ( i < 20 )
        {
			f = (b & c) | (~b & d);
			k = 0x5A827999;
		}
        else if ( i < 40 )
        {
			f = b ^ c ^ d;
			k = 0x6ED9EBA1;
		}
        else if ( i < 60 )
        {
			f = (b & c) | (b & d) | (c & d);
			k = 0x8F1BBCDC;
		}
        else
        {
			f = b ^ c ^ d;
			k = 0xCA62C1D6;
		}

		unsigned t = rotl(a, 5) + f + e + k + w[i];
		e = d;
		d = c;
		c = rotl(b, 30);
		b = a;
		a = t;
	}

	h[0] += a;
	h[1] += b;
	h[2] += c;
	h[3] += d;
	h[4] += e;
}

void sha1_msg(unsigned char* m, int size, unsigned hash[5], int rounds)
{
    /* Initialize variables */
    unsigned h[5];
    h[0] = 0x67452301;
    h[1] = 0xEFCDAB89;
    h[2] = 0x98BADCFE;
    h[3] = 0x10325476;
    h[4] = 0xC3D2E1F0;

    unsigned long long ml = size * 8; //message length in bits (always a multiple of the number of bits in a character).

    /* Message padding pre-process */
    vector<unsigned char> M(m, m+size);
    M.push_back(0x80);
    while( M.size() % 64 != 56 ) M.push_back(0x00);
    M.push_back((ml >> 56) & 0xFF);
    M.push_back((ml >> 48) & 0xFF);
    M.push_back((ml >> 40) & 0xFF);
    M.push_back((ml >> 32) & 0xFF);
    M.push_back((ml >> 24) & 0xFF);
    M.push_back((ml >> 16) & 0xFF);
    M.push_back((ml >> 8) & 0xFF);
    M.push_back((ml) & 0xFF);
    size = M.size();

    unsigned w[80];

    /*  Process the message in successive 512-bit chunks:
        break message into 512-bit chunks */
    for( int chunk=0; chunk<size; chunk+=64 )  /* for each chunk */
    {
        /* break chunk into sixteen 32-bit big-endian words w[i], 0 <= i <= 15 */
        for( int i=0; i<16; i++ )
        {
            w[i] = 0;
            for( int j=0; j<4; j++ )
                w[i] = (w[i] << 8) | M[chunk + i*4 + j];
        }
        
        sha1_comp(w, h, rounds, true);
    }

    for( int i=0; i<5; i++ )
        hash[i] = h[i];
}


void sha256_comp(unsigned w[64], unsigned H[8], int rounds, bool chaining)
{
    /*Note 1: All variables are 32 bit unsigned integers and addition is calculated modulo 232
      Note 2: For each round, there is one round constant k[i] and one entry in the message schedule array w[i], 0 ≤ i ≤ 63
      Note 3: The compression function uses 8 working variables, a through h
      Note 4: Big-endian convention is used when expressing the constants in this pseudocode,
      and when parsing message block data from bytes to words, for example,
      the first word of the input message "abc" after padding is 0x61626380
      */
    /*Initialize hash values:
      (first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):*/
    if ( !chaining )
    {
        H[0] = 0x6a09e667;
        H[1] = 0xbb67ae85;
        H[2] = 0x3c6ef372;
        H[3] = 0xa54ff53a;
        H[4] = 0x510e527f;
        H[5] = 0x9b05688c;
        H[6] = 0x1f83d9ab;
        H[7] = 0x5be0cd19;
    }

    /*Initialize array of round constants:
      (first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):*/
    unsigned k[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };


    /*    Extend the first 16 words into the remaining 48 words w[16..63] of the message schedule array: */
    for( int i=16; i<rounds; i++ )
    {
        unsigned s0 = rotr(w[i-15], 7) ^ rotr(w[i-15], 18) ^ shr(w[i-15], 3);
        unsigned s1 = rotr(w[i-2], 17) ^ rotr(w[i-2], 19) ^ shr(w[i-2], 10);
        w[i] = w[i-16] + s0 + w[i-7] + s1;
    }


    /*    Initialize working variables to current hash value: */
    unsigned a = H[0];
    unsigned b = H[1];
    unsigned c = H[2];
    unsigned d = H[3];
    unsigned e = H[4];
    unsigned f = H[5];
    unsigned g = H[6];
    unsigned h = H[7];

    /*    Compression function main loop: */
    for( int i=0; i<rounds; i++ )
    {
        unsigned S1 = rotr(e, 6) ^ rotr(e, 11) ^ rotr(e, 25);
        unsigned ch = (e & f) ^ ((~ e) & g);
        unsigned temp1 = h + S1 + ch + k[i] + w[i];
        unsigned S0 = rotr(a, 2) ^ rotr(a, 13) ^ rotr(a, 22);
        unsigned maj = (a & b) ^ (a & c) ^ (b & c);
        unsigned temp2 = S0 + maj;

        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    /*    Add the compressed chunk to the current hash value: */
    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
    H[4] += e;
    H[5] += f;
    H[6] += g;
    H[7] += h;
}

void sha256_msg(unsigned char* m, int size, unsigned hash[8], int rounds)
{
    /*Note 1: All variables are 32 bit unsigned integers and addition is calculated modulo 232
      Note 2: For each round, there is one round constant k[i] and one entry in the message schedule array w[i], 0 ≤ i ≤ 63
      Note 3: The compression function uses 8 working variables, a through h
      Note 4: Big-endian convention is used when expressing the constants in this pseudocode,
      and when parsing message block data from bytes to words, for example,
      the first word of the input message "abc" after padding is 0x61626380
      */
    /*Initialize hash values:
      (first 32 bits of the fractional parts of the square roots of the first 8 primes 2..19):*/
    unsigned H[8];
    H[0] = 0x6a09e667;
    H[1] = 0xbb67ae85;
    H[2] = 0x3c6ef372;
    H[3] = 0xa54ff53a;
    H[4] = 0x510e527f;
    H[5] = 0x9b05688c;
    H[6] = 0x1f83d9ab;
    H[7] = 0x5be0cd19;

    /*Initialize array of round constants:
      (first 32 bits of the fractional parts of the cube roots of the first 64 primes 2..311):*/
    unsigned k[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2 };

    /*Pre-processing:
      append the bit '1' to the message
      append k bits '0', where k is the minimum number >= 0 such that the resulting message
      length (modulo 512 in bits) is 448.
      append length of message (without the '1' bit or padding), in bits, as 64-bit big-endian integer
      (this will make the entire post-processed length a multiple of 512 bits) */
    unsigned long long ml = size * 8;

    int i = size;
    vector<unsigned char> M(m, m+size);
    M.push_back(0x80);
    while( M.size() % 64 != 56 ) M.push_back(0x00);
    M.push_back((ml >> 56) & 0xFF);
    M.push_back((ml >> 48) & 0xFF);
    M.push_back((ml >> 40) & 0xFF);
    M.push_back((ml >> 32) & 0xFF);
    M.push_back((ml >> 24) & 0xFF);
    M.push_back((ml >> 16) & 0xFF);
    M.push_back((ml >> 8) & 0xFF);
    M.push_back((ml) & 0xFF);
    size = M.size();

    unsigned w[64];

    /* Process the message in successive 512-bit chunks:
       break message into 512-bit chunks */
    for( int chunk=0; chunk<size; chunk+=64 )
    {
        /*    create a 64-entry message schedule array w[0..63] of 32-bit words
              (The initial values in w[0..63] don't matter, so many implementations zero them here)
              copy chunk into first 16 words w[0..15] of the message schedule array*/

        for( int i=0; i<16; i++ )
        {
            w[i] = 0;
            for( int j=0; j<4; j++ )
                w[i] = (w[i] << 8) | M[chunk + i*4 + j];
        }

        sha256_comp(w, H, rounds, true);
    }

    for( int i=0; i<8; i++ )
        hash[i] = H[i];
}
