#include <stdio.h>
#include <stdint.h>	
#include "SM4_BS.h"
#ifndef BS_64bit
#define BS_256bit
#include <immintrin.h>
#define _mm256_set_m128i(v0, v1)  _mm256_insertf128_si256(_mm256_castsi128_si256(v1), (v0), 1)
#define _mm256_setr_m128i(v0, v1) _mm256_set_m128i((v1), (v0))
typedef __m256i bit_t;//__m256i bit_t;
typedef struct {
  bit_t b0;
  bit_t b1;
  bit_t b2;
  bit_t b3;
  bit_t b4;
  bit_t b5;
  bit_t b6;
  bit_t b7;
} bits;


void hi();
void BS_init_M(__m128i* M);
void SM4_BS_enc(__m128i* M,__m256i* N);
// void SM4_BS_enc();
void BS_TRANS();
void BS_TRANS_128x256(__m128i* M,__m256i* N);
void BS_TRANS_inv();
void BS_TRANS_VER_128x256(__m256i* N,__m128i* M);
static unsigned long sm4CalciRK(unsigned long ka);
static unsigned char sm4Sbox(unsigned char inch);
void Sm4_BoolFun(bits in, bit_t *out0, bit_t *out1, bit_t *out2, bit_t *out3, bit_t *out4, bit_t *out5, bit_t *out6, bit_t *out7);
// void BS_iteration();
void BS_iteration(__m256i* N);
void S_box(int round);
void benchmark_sm4_bs_encrypt();
/*
 * 32-bit integer manipulation macros (big endian)
 */
#ifndef GET_ULONG_BE
#define GET_ULONG_BE(n,b,i)                             \
{                                                       \
    (n) = ( (unsigned long) (b)[(i)    ] << 24 )        \
        | ( (unsigned long) (b)[(i) + 1] << 16 )        \
        | ( (unsigned long) (b)[(i) + 2] <<  8 )        \
        | ( (unsigned long) (b)[(i) + 3]       );       \
}
#endif

#ifndef PUT_ULONG_BE
#define PUT_ULONG_BE(n,b,i)                             \
{                                                       \
    (b)[(i)    ] = (unsigned char) ( (n) >> 24 );       \
    (b)[(i) + 1] = (unsigned char) ( (n) >> 16 );       \
    (b)[(i) + 2] = (unsigned char) ( (n) >>  8 );       \
    (b)[(i) + 3] = (unsigned char) ( (n)       );       \
}
#endif

/*
 *rotate shift left marco definition
 *
 */
#define  SHL(x,n) (((x) & 0xFFFFFFFF) << n)
#define ROTL(x,n) (SHL((x),n) | ((x) >> (32 - n)))

#define SWAP(a,b) { uint64_t t = a; a = b; b = t; t = 0; }

#endif