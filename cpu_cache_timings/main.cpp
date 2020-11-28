/*
CPU: AMD FX-8350 Eight-Core @ 8x 4GHz

L2 Cache Size - 8 MB
L3 Cache Size - 8 MB

Stress test
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <thread>
#include <cmath>
#include "timer.h"

//#define SIZE 2147483648
#define SIZE 200000000

static void multiply (float *a, float *b, float *c, long size, long chunks)
{

   long chunk_length = size / chunks;
   long remainder    = size % chunks;

   float *a_ptr = a;
   float *b_ptr = b;
   float *c_ptr = c;

   for (long chunk = 0; chunk < chunks; chunk++)
      for (long ind = 0; ind < chunk_length; ind++)
         c_ptr[ind] = a_ptr[ind] * b_ptr[ind];

   for (long ind = 0; ind < remainder; ind++)
      c_ptr[ind] = a_ptr[ind] * b_ptr[ind];
}

static void fill (float *a, float *b, long length)
{
   const long pieces = 100;

   long chunk_length = length / pieces;
   long remainder    = length % pieces;

   float *a_ptr = a;
   float *b_ptr = b;

   for (long part = 0; part < pieces; part++)
   {
      if (part == pieces - 1) chunk_length += remainder;

      for (long ind = 0; ind < chunk_length; ind++) a_ptr[ind] = (float)(rand() % 10000) / 10000.0f;
      for (long ind = 0; ind < chunk_length; ind++) b_ptr[ind] = (float)(rand() % 10000) / 10000.0f;

      a_ptr += chunk_length;
      b_ptr += chunk_length;

      if (part % 5 == 0)
         std::cout << (float)(part + 1) / (float)pieces << " fraction of the way there" << std::endl;
   }
}

int main ()
{
   float *a = new float[SIZE];
   float *b = new float[SIZE];
   float *c = new float[SIZE];

   // Touch all the memory first to assure contiguous allocation
   for (long ind = 0; ind < SIZE; ind++) a[ind] = 0.0f;
   for (long ind = 0; ind < SIZE; ind++) b[ind] = 0.0f;
   for (long ind = 0; ind < SIZE; ind++) c[ind] = 0.0f;

   long start;
   float time;

   start = startTime ();

   int num_threads = std::thread::hardware_concurrency ();

   long sub_size  = SIZE / num_threads;
   long remainder = SIZE % num_threads;

   std::thread threads[num_threads];

   for (int thread = 0; thread < num_threads; thread++)
   {
      int length = sub_size;
      if (thread == num_threads - 1) length += remainder;

      threads[thread] =
         std::thread (
               fill,
               a + thread * sub_size,
               b + thread * sub_size,
               length);

      std::cout << "spawned thread " << thread << std::endl;
   }

   for (int thread = 0; thread < num_threads; thread++)
      threads[thread].join ();

   time = endTime (start);

   std::cout << "initialization = " << time << std::endl;

   std::ofstream results;
   results.open ("results.txt");

   results << "size (bytes)" << std::setw (16) << "time (seconds)" << std::setw (16) << "std" << std::endl;
   results << "============" << std::setw (16) << "==============" << std::setw (16) << "===" << std::endl;

   int  step_size  = 1;
   long chunk_size = 1;

   // Processing time (hours) ~= 7.0 * max_iterations * total_stats / 60 / 60
   const int max_iterations = 600;
   const int total_stats    = 8;

   for (int iteration = 0; iteration < max_iterations && chunk_size <= SIZE; iteration++)
   {
      float S  = 0.0f;
      float SS = 0.0f;

      long chunks = SIZE / chunk_size;

      int count;
      for (count = 0; count < total_stats; count++)
      {
         start = startTime ();

         // Perform an out-of-place c = a * b multiply
         // for a total of "SIZE" times with each over
         // a "chunk" number of elements length
         multiply (a, b, c, SIZE, chunks);

         time = endTime (start);

         S  += time;
         SS += time * time;
      }

      float ave = S / (float)count;
      float var = SS / (float)count - ave * ave;
      float sig = var > 0.00000f ? sqrtf (var) : 0.0f;

      results
         << chunk_size * 3 * 4
         << ", "
         << ave
         << ", "
         << sig
         << std::endl;

      if (iteration % 100 == 0 && iteration > 0) step_size *= 10;
      chunk_size += step_size;
   }

   results.close ();

   delete[] a;
   delete[] b;
   delete[] c;

   return 0;
}
