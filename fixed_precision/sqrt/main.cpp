// Q15 fixed-point square root example
#include <iostream>
#include <cmath>

int main ()
{

   short x;

   x = (short)((0.44159f) * 32767.0f); // 2**15 - 1

   short y;

   short low = 0;
   short high = 0x7FFF;

   short tol = 100;
   for (int itt = 0; itt < 100; itt++) {
      y = (short)(((int)high + (int)low) / 2);
      int val = (int)y * (int)y;
      val >>= 15;
      if ((short)val > x) high = y;
      if ((short)val < x) low  = y;
      tol = high - low;
   }

   std::cout << "sqrt(" << (float)x / 32767.0f << ") = " << (float)y / 32767.0f << std::endl;
   std::cout << "sqrt(" << (float)x / 32767.0f << ") = " << sqrt((float)x / 32767.0f) << std::endl;

   return 0;
}
