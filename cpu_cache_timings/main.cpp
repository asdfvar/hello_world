/*
CPU: AMD FX-8350 Eight-Core @ 8x 4GHz

L2 Cache Size - 8 MB
L3 Cache Size - 8 MB

Stress test
*/

#include <iostream>
#include <cstdlib>
#include <iomanip>
#include <thread>
#include "timer.h"

#define SIZE 2147483648

static void multiply (float *a, float *b, float *c, long size, long chunks)
{

   long chunk_length = size / chunks;
   long remainder    = size % chunks;

   float *a_ptr = a;
   float *b_ptr = b;
   float *c_ptr = c;

   for (long chunk = 0; chunk < chunks; chunk++)
   {
      for (long ind = 0; ind < chunk_length; ind++)
      {
         c_ptr[ind] = a_ptr[ind] * b_ptr[ind];
      }
   }

   for (long ind = 0; ind < remainder; ind++)
   {
      c_ptr[ind] = a_ptr[ind] * b_ptr[ind];
   }
}

static void fill (float *a, float *b, long length)
{
   const long pieces = 100;

   long chunk_length    = length / pieces;
   long remainder = length % pieces;

   float *a_ptr = a;
   float *b_ptr = b;

   for (long part = 0; part < pieces; part++)
   {
      if (part == pieces - 1) chunk_length += remainder;

      for (long ind = 0; ind < chunk_length; ind++) a_ptr[ind] = (float)(rand() % 10000) / 10000.0f;
      for (long ind = 0; ind < chunk_length; ind++) b_ptr[ind] = (float)(rand() % 10000) / 10000.0f;

      a_ptr += chunk_length;
      b_ptr += chunk_length;

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

   int num_threads = 8;

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

   for (long chunk_size = 1; chunk_size <= SIZE; chunk_size *= 2)
   {
      if (chunk_size > SIZE) chunk_size = SIZE;

      long chunks = SIZE / chunk_size;

      start = startTime ();

      multiply (a, b, c, SIZE, chunks);

      time = endTime (start);

      std::cout << "time for chunk size "
         << chunk_size * 3 * 4
         << " bytes " << std::setw (10)
         <<  " = " << time << std::endl;
   }

   delete[] a;
   delete[] b;
   delete[] c;

   return 0;
}
