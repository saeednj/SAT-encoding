#ifndef _UTIL_H_
#define _UTIL_H_

unsigned rotl(unsigned x, int n);
unsigned rotr(unsigned x, int n);
unsigned shr(unsigned x, int n);
void sha1_comp(unsigned w[80], unsigned h[5], int rounds = 80, bool chaining = false);
void sha1_msg(unsigned char* m, int size, unsigned hash[5], int rounds);

void sha256_comp(unsigned w[64], unsigned H[8], int rounds = 64, bool chaining = false);
void sha256_msg(unsigned char* m, int size, unsigned hash[8], int rounds);

#endif
