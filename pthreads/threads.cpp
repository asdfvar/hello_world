#include <pthread.h>
#include <iostream>

#define NUM_THREADS 8
#define LOAD_ITERS  1000000
#define NUM_LOADS   10

typedef struct ARGS {
   int                thread_num;
   pthread_barrier_t* barrier;
   float*             data;
   bool               done;
} ARGS;

void* function (void* input) {
   ARGS* arg = (ARGS*)input;

   pthread_barrier_t* barrier = arg->barrier;
   bool* done = &arg->done;

   float* data = arg->data;

   // wait for attributes
   pthread_barrier_wait (barrier);

   while (!*done) {

      // wait for signal to compute
      pthread_barrier_wait (barrier);

      for (int iter = 0; iter < LOAD_ITERS; iter++)
         for (int ind = 0; ind < 1024; ind++)
         {
            data[ind] = 3.14159f * 2.718281f + 1.168034f;
         }

      // wait for signal that we are done computing
      pthread_barrier_wait (barrier);

      // wait for attributes
      pthread_barrier_wait (barrier);

   }

}

int main (void)
{

   pthread_t threads[NUM_THREADS];
   ARGS args[NUM_THREADS];
   pthread_barrier_t barrier;
   pthread_barrier_init (&barrier, NULL, NUM_THREADS + 1);

   float* data = new float[1024*NUM_THREADS];


   for (int thread = 0; thread < NUM_THREADS; thread++)
   {
      args[thread].thread_num = thread;
      args[thread].barrier    = &barrier;
      args[thread].data       = data + 1024 * thread;
      args[thread].done       = false;
      pthread_create (&threads[thread], NULL, function, (void*)&args[thread]);
   }

   for (int iter = 0; iter < NUM_LOADS; iter++)
   {
      // tell thread attributes are ready
      pthread_barrier_wait (&barrier);

      // tell thread to compute
      pthread_barrier_wait (&barrier);

      // wait for threads to finish computing
      pthread_barrier_wait (&barrier);
   }

   // close down
   for (int thread = 0; thread < NUM_THREADS; thread++)
   {
      args[thread].done = true;
   }
   // tell threads close-down attributes are ready
   pthread_barrier_wait (&barrier);

   for (int thread = 0; thread < NUM_THREADS; thread++)
   {
      pthread_join (threads[thread], NULL);
   }

   float min = 9999.0f;
   float max = -1.0f;
   for (int ind = 0; ind < 1024*NUM_THREADS; ind++) {
      if (data[ind] < min) min = data[ind];
      if (data[ind] > max) max = data[ind];
   }
   std::cout << "min = " << min << " max = " << max << std::endl;

   std::cout << "done" << std::endl;

   return 0;
}
