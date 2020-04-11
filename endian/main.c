#include <iostream>

int main (void)
{
   int x = 65535;

   char *c0, *c1, *c2, *c3;

   c0 = (char*)&x;

   c1 = c0 + 1;
   c2 = c1 + 1;
   c3 = c2 + 1;

   std::cout
      << " c0 = " << *c0
      << " c1 = " << *c1
      << " c2 = " << *c2
      << " c3 = " << *c3 << std::endl;

   for (int ind = 0, k = 1; ind < 32; k *= 2, ind++)
   {
      if (ind % 4 == 0) std::cout << " ";
      std::cout << ((x & k) != 0) ? 1 : 0;
   }
   std::cout << std::endl;

   return 0;
}
