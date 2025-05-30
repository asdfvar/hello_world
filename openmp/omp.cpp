#include <iostream>
#include <omp.h>

int main () {

#pragma omp parallel
   {
#pragma omp critical
      {
         std::cout << "Thread " << omp_get_thread_num () << std::endl;
      }
   }
   std::cout << "Done" << std::endl;

   return 0;
}
