#include <iostream>
#include <stdlib.h>

extern "C" {
#include <emmintrin.h>
#include <mmintrin.h>
#include <pmmintrin.h>
#include <immintrin.h>
#include <smmintrin.h>
}

static float *byte_align (float *src, size_t byte)
{
   char *asrc = (char*)src;
   while ((size_t)asrc % byte != 0) asrc++;
   return (float*)asrc;
}

int main (void)
{

   float a[16];
   float b[16];
   float c[16];
   float d[16];

   // Point aliases to the next 32-byte boundary
   float *ap = byte_align (a, 32);
   float *bp = byte_align (b, 32);
   float *cp = byte_align (c, 32);
   float *dp = byte_align (d, 32);

   for (int ind = 0; ind < 8; ind++) {
      ap[ind] = (float) (rand () % 1000) / 100;
      bp[ind] = (float) (rand () % 1000) / 100;
      cp[ind] = (float) (rand () % 1000) / 100;
   }

   std::cout << "A = ";
   for (int ind = 0; ind < 8; ind++) {
      std::cout << ap[ind] << ", ";
   }
   std::cout << std::endl;

   std::cout << "B = ";
   for (int ind = 0; ind < 8; ind++) {
      std::cout << bp[ind] << ", ";
   }
   std::cout << std::endl;

   std::cout << "C = ";
   for (int ind = 0; ind < 8; ind++) {
      std::cout << cp[ind] << ", ";
   }
   std::cout << std::endl;

   __m256 *ma = (__m256*)ap;
   __m256 *mb = (__m256*)bp;
   __m256 *mc = (__m256*)cp;
   __m256 *md = (__m256*)dp;

   // Perform an element-wise fused-multiply add (D = A*B + C)
   *md = _mm256_fmadd_ps (*ma, *mb, *mc);

   std::cout << "A*B + C = ";
   for (int ind = 0; ind < 8; ind++) {
      std::cout << dp[ind] << ", ";
   }
   std::cout << std::endl;

   return 0;
}
