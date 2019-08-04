#include <pthread.h>
#include <iostream>
#include <Python.h>

#define NUM_THREADS 4

typedef struct Arg {
   int                thread_id;
   pthread_barrier_t *barrier;
   pthread_mutex_t   *lock;
} Arg;

void *function (void *argument) {
   Arg *arg = (Arg*)argument;

   pthread_barrier_wait (arg->barrier);

   pthread_mutex_lock (arg->lock);
   Py_Initialize ();
   pthread_mutex_unlock (arg->lock);

   pthread_mutex_lock (arg->lock);
   std::cout << "message from " << arg->thread_id << std::endl;
   pthread_mutex_unlock (arg->lock);

   pthread_mutex_lock (arg->lock);
   Py_Finalize ();
   pthread_mutex_unlock (arg->lock);

}

int main()
{
   Arg args[NUM_THREADS];

   pthread_t         threads[NUM_THREADS];
   pthread_barrier_t barrier;
   pthread_mutex_t   lock;

   pthread_barrier_init (&barrier, NULL, NUM_THREADS + 1);
   pthread_mutex_init (&lock, NULL);

   for (int thread = 0; thread < NUM_THREADS; thread++)
   {
      args[thread].thread_id = thread;
      args[thread].barrier   = &barrier;
      args[thread].lock      = &lock;

      pthread_create (&threads[thread], NULL, function, (void*)&args[thread]);
   }

   pthread_barrier_wait (&barrier);

   for (int thread = 0; thread < NUM_THREADS; thread++)
   {
      pthread_join (threads[thread], NULL);
   }

   pthread_barrier_destroy (&barrier);
   pthread_mutex_destroy (&lock);

   return 0;
}
