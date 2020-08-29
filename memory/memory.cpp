#include <iostream>
#include <unistd.h>
#include <string.h>

#define SIZE 8000000000

int main ()
{

   float *x = new float[SIZE];

   memset (x, 0, SIZE);
   //for (int k = 0; k < SIZE / sizeof (float); k++) x[k] = 0.0f;

//   sleep (10);

   return 0;
}
