#include <sys/time.h>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
#include <string.h>

#define N 10000000

int main ()
{

   struct timeval start, end;

   float *x = new float[N];
   float *y = new float[N];
   double time;

   std::ofstream myfile;

   gettimeofday (&start, nullptr);
   myfile.open ("data", std::ios::binary);
   gettimeofday (&end, nullptr);
   time = (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000.0f;
std::cout << __FILE__ << __LINE__ << ":time = " << time << std::endl;

   sleep (5.0f);

   gettimeofday (&start, nullptr);
   for (int ind = 0; ind < N; ind++) x[ind] = (float)(rand () % 100) / 100.0f;
   gettimeofday (&end, nullptr);
   time = (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000.0f;
std::cout << __FILE__ << __LINE__ << ":time = " << time << std::endl;

   sleep (5.0f);

   gettimeofday (&start, nullptr);
   for (int ind = 0; ind < N; ind++) myfile << x[ind];
   gettimeofday (&end, nullptr);
   time = (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000.0f;
std::cout << __FILE__ << __LINE__ << ":time = " << time << std::endl;

   sleep (5.0f);

   gettimeofday (&start, nullptr);
   myfile.close ();
   gettimeofday (&end, nullptr);
   time = (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000.0f;
std::cout << __FILE__ << __LINE__ << ":time = " << time << std::endl;

   sleep (5.0f);

   gettimeofday (&start, nullptr);
   memcpy (y, x, N * sizeof (float));
   gettimeofday (&end, nullptr);
   time = (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000.0f;
std::cout << __FILE__ << __LINE__ << ":time = " << time << std::endl;

   sleep (5.0f);

   gettimeofday (&start, nullptr);
   for (int ind = 0; ind < N; ind++) y[ind] = x[ind];
   gettimeofday (&end, nullptr);
   time = (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000.0f;
std::cout << __FILE__ << __LINE__ << ":time = " << time << std::endl;

   sleep (5.0f);

   gettimeofday (&start, nullptr);
   delete[] x;
   delete[] y;
   gettimeofday (&end, nullptr);
   time = (end.tv_sec * 1000000 + end.tv_usec - start.tv_sec * 1000000 - start.tv_usec) / 1000000.0f;
std::cout << __FILE__ << __LINE__ << ":time = " << time << std::endl;

   sleep (5.0f);

   return 0;
}
