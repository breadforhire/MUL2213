#include "immintrin.h"
#include "avx2intrin.h"
#include <emmintrin.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/*MUL2213 algorithm*/

/*magic number for 2^56 for 64 bit arthitecture, however we are using 32 bit for more expermiental reasons I will implement 64 bit but just not right now*/
#define c_t  72057594037927936


/*ok now we have to change this to 64 bit*/


#define PRINT(vector) \
    do { \
        for (int i = 0; i < 4; i++) { \
            printf("\n %d --> %d \n", vector[i], i); \
        } \
    } while (0)


/*I dont know if this extraction will work*/



__attribute__((__aligned__(32)))
static const int32_t x_n[4] = {0, 1, 2, 3};

__attribute__((__aligned__(32)))
static int32_t y_n [4] = {0, 1, 2, 3};


__attribute__((__aligned__(32)))
static int32_t c_24 [4] = {24, 24, 24, 24};




void A()
{

 uint32_t temp[4];
 __m128i x = _mm_load_si128((__m128i*)x_n);
 __m128i y = _mm_load_si128((__m128i*)  y_n);
 __m128i c_24_a = _mm_load_si128((__m128i*)c_24);

  /*a registers*/
  __m128i a0 = _mm_set1_epi32(x_n[0] * y_n[0]);
  __m128i a1 = _mm_set1_epi32(x_n[1] * y_n[1]);
  __m128i a2 = _mm_set1_epi32(x_n[2] * y_n[2]);
  __m128i a3 = _mm_set1_epi32(x_n[3] * y_n[3]);

  /*setting up the b registers*/
  /*13*/
  __m128i b2 = _mm_add_epi32(a3, a2);
  /*14*/
  __m128i b1 = _mm_add_epi32(b2, a1);
  __m128i b0 = _mm_add_epi32(b1, a0);

  /*setting up z registers*/
  __m128i z0;
  __m128i z1;
  __m128i z2;
  __m128i z3;



    /*extraction of low 32 bits*/

  /*setting up t registers*/
  __m128i t0;
  __m128i t1;

 /* t0 ← b0 − (x0 − x3)(y0 − y3) − (x1 − x2)(y1 − y2) */
  t0 = _mm_mullo_epi32 (_mm_sub_epi32(_mm_set1_epi32(x_n[0]), _mm_set1_epi32(x_n[3])), _mm_sub_epi32(_mm_set1_epi32(y_n[0]), _mm_set1_epi32(y_n[3])));
  t0 = _mm_sub_epi32(b0, t0);
  t0 = _mm_sub_epi32(t0,  _mm_mullo_epi32 (_mm_sub_epi32(_mm_set1_epi32(x_n[1]), _mm_set1_epi32(x_n[2])), _mm_sub_epi32(_mm_set1_epi32(y_n[1]), _mm_set1_epi32(y_n[2]))));


  /*z3 ← t0 mod t*/
  z3 = _mm_set1_epi32(_mm_cvtsi128_si32 (t0) % c_t);

  /* a0 + 24(b1 − (x1 − x3)(y1 − y3) + (t0 >> 56)) */
  t1 = _mm_mullo_epi32 (_mm_sub_epi32(_mm_set1_epi32(x_n[1]), _mm_set1_epi32(x_n[3])), _mm_sub_epi32(_mm_set1_epi32(y_n[1]), _mm_set1_epi32(y_n[3])));
  t1 = _mm_sub_epi32(b1, t1);
  t1 = _mm_add_epi32(a0, _mm_mullo_epi32(t1, c_24_a));
  t1 =  _mm_add_epi32(_mm_slli_epi64(t0, 56), t1);

 _mm_storeu_si128((__m128i*)temp, t1);
 z0 = _mm_set1_epi32 (_mm_cvtsi128_si32 (t1) % c_t);
 a0 = _mm_add_epi32(a0, a1);

 /* t0 ← a0 − (x0 − x1)(y0 − y1) + 24(b2 − (x2 − x3)(y2 − y3)) + (t1 >> 56) */
 t0 = _mm_add_epi32(_mm_slli_epi64(t1, 56), t0);
 t0 = _mm_sub_epi32(b2, _mm_mullo_epi32 (_mm_sub_epi32(_mm_set1_epi32(x_n[2]), _mm_set1_epi32(x_n[3])), _mm_sub_epi32(_mm_set1_epi32(y_n[2]), _mm_set1_epi32(y_n[3]))));
 t0 = _mm_mullo_epi32(t0, c_24_a);
 t0  = _mm_add_epi32(_mm_sub_epi32(a0, _mm_mullo_epi32 (_mm_sub_epi32(_mm_set1_epi32(x_n[0]), _mm_set1_epi32(x_n[1])), _mm_sub_epi32(_mm_set1_epi32(y_n[0]), _mm_set1_epi32(y_n[1])))), t0);

 t0 = _mm_add_epi32(t0, _mm_set1_epi32(_mm_cvtsi128_si32(t1) >> 58));
 z1 = _mm_set1_epi32( _mm_cvtsi128_si32 (t0) % c_t);

 /* t1 ← a0 + a2 + 24a3 − (x0 − x2)(y0 − y2) + (t0 >> 56) */
 t1 = _mm_add_epi32(a0, a2);
 t1 = _mm_add_epi32(_mm_mullo_epi32(c_24_a, a3), t1);
 t1 = _mm_sub_epi32( t1,  _mm_mullo_epi32 (_mm_sub_epi32(_mm_set1_epi32(x_n[0]), _mm_set1_epi32(x_n[1])), _mm_sub_epi32(_mm_set1_epi32(y_n[0]), _mm_set1_epi32(y_n[1]))));

 t1 = _mm_add_epi32( t1, _mm_set1_epi32(_mm_cvtsi128_si32 (t0) >> 56));

 _mm_storeu_si128((__m128i*)temp, t1);
 z2 = _mm_set1_epi32(_mm_cvtsi128_si32 (t1) % c_t);


 /* we can store this in a variable, so we do not have to do the operations twice but results are negilibile as of last benchmarking */
 t0 = _mm_add_epi32(z3, _mm_slli_epi64(t1, 58));
 _mm_storeu_si128((__m128i*)temp, t0);
 z3 = _mm_set1_epi32(temp[0] % c_t);
 z0 = _mm_add_epi32(z0, _mm_mullo_epi32(c_24_a, _mm_slli_epi64(t1, 58)));










}

int main()
{
 A();
}
