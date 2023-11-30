#include "immintrin.h"
#include "avx2intrin.h"
#include <emmintrin.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*MUL2213 algorithm*/

#define c_t 2

__attribute__((__aligned__(32)))
static const int32_t x_n[4] = {0, 1, 2, 3};

__attribute__((__aligned__(32)))
static int32_t y_n [4] = {0, 1, 2, 3};

#define PRINT(vector) \
    do { \
        for (int i = 0; i < 4; i++) { \
            printf("\n%d --> %d \n", vector[i], i); \
        } \
    } while (0)

void A()
{

 uint32_t temp[4];
 __m128i x = _mm_load_si128((__m128i*)x_n);
 __m128i y = _mm_load_si128((__m128i*)  y_n);

  /*a registers*/
  __m128i a0 = _mm_set1_epi32(x_n[0] * y_n[0]);
  __m128i a1 = _mm_set1_epi32(x_n[1] * y_n[1]);
  __m128i a2 = _mm_set1_epi32(x_n[2] * y_n[2]);
  __m128i a3 = _mm_set1_epi32(x_n[3] * y_n[3]);

  /*setting up the b registers*/
  __m128i b2 = _mm_add_epi32(a3, a2);
  __m128i b1 = _mm_add_epi32(b2, a1);
  __m128i b0 = _mm_add_epi32(b1, a0);

 /* t0 ← b0 − (x0 − x3)(y0 − y3) − (x1 − x2)(y1 − y2) */
  __m128i t0 = _mm_mullo_epi32 (_mm_sub_epi32(_mm_set1_epi32(x_n[0]), _mm_set1_epi32(x_n[3])), _mm_sub_epi32(_mm_set1_epi32(y_n[0]), _mm_set1_epi32(y_n[3])));
  t0 = _mm_sub_epi32(b0, t0);
  t0 = _mm_sub_epi32(t0,  _mm_mullo_epi32 (_mm_sub_epi32(_mm_set1_epi32(x_n[0]), _mm_set1_epi32(x_n[3])), _mm_sub_epi32(_mm_set1_epi32(y_n[0]), _mm_set1_epi32(y_n[3]))));
  _mm_store_si128((__m128i*)temp, t0);

  /*z3 ← t0 mod t*/
  __m128i z3 = _mm_set1_epi32(temp[0] % c_t);








}

int main()
{
 A();
}
