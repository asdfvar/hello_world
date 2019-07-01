#include "ctopy.h"
#include <iostream>

int main ()
{

   int stat = 0;

   int x = 7;
   int y = 9;

   // instantiate an instance for specified python function
   C_TO_PY py_multiplier (
         ".",          // python path
         "arithmetic", // module name
         "multiply",   // function name
         2);           // number of arguments

   // set the arguments
   py_multiplier.set_arg (x);
   py_multiplier.set_arg (y);

   // execute the python function
   py_multiplier.execute ();

   int z[4] = { 3, 1, 4, 1 };

   // instantiate an instance for specified python function
   C_TO_PY py_summation (
         ".",          // python path
         "arithmetic", // module name
         "summation",  // function name
         1);           // number of arguments

   // set the argument
   py_summation.set_arg (z, 4);

   // execute the python function
   py_summation.execute ();

   // instantiate an instance for specified python function
   C_TO_PY py_sub_summation (
         ".",              // python path
         "arithmetic",     // module name
         "sub_summation",  // function name
         2);               // number of arguments

   // set the arguments
   py_sub_summation.set_arg (z, 4);
   py_sub_summation.set_arg (2);

   // execute the python function
   py_sub_summation.execute ();

   return 0;

}
