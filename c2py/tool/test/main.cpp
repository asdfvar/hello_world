#include "ctopy.h"
#include <iostream>

int main ()
{

   int stat = 0;

   int x = 7;
   int y = 9;

#if 1
   C_TO_PY py_multiplier (
         ".",          // python path
         "arithmetic", // module name
         "multiply",   // function name
         2);           // number of arguments

   stat |= py_multiplier.set_arg (x);
   stat |= py_multiplier.set_arg (y);

   py_multiplier.execute ();
#endif

#if 1
   int z[4] = { 3, 1, 4, 1 };

   C_TO_PY py_summation (
         ".",          // python path
         "arithmetic", // module name
         "summation",  // function name
         1);           // number of arguments

   stat |= py_summation.set_arg (z, 4);

   py_summation.execute ();
#endif

   if (stat != 0) std::cout << "error" << std::endl;

   return 0;

}
