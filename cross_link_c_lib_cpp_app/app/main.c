#include "arithmetic.h"
#include <iostream>

int main ()
{
   int a = 7;
   int b = 11;

   int c = add (a, b);

   std::cout << a << " + " << b << " = " << c << std::endl;

   return 0;
}
