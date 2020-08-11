#ifndef _UTIL_H_
#define _UTIL_H_

unsigned rotl(unsigned x, int n);
unsigned rotr(unsigned x, int n);
unsigned shr(unsigned x, int n);
void sha1_comp(unsigned *w, unsigned *h, int rounds = 80, bool chaining = false);
void sha1_msg(unsigned char* m, int size, unsigned *hash, int rounds);

void sha256_comp(unsigned *w, unsigned *H, int rounds = 64, bool chaining = false);
void sha256_msg(unsigned char* m, int size, unsigned *hash, int rounds);

void md4_comp(unsigned *w, unsigned *h, int rounds = 48, bool chaining = false);
#endif
