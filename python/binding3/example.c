#include <stdio.h>

extern "C" {

   int add (int a, int b) {
      return a + b;
   }

   void greet (const char* name) {
      printf ("Hello, %s!\n", name);
   }

}
