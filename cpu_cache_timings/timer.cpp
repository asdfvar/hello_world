#include "timer.h"
#include <sys/time.h>
#include <iostream>

long startTime (void) {
   struct timeval start;

   gettimeofday (&start, nullptr);

   return start.tv_sec * 1000000 + start.tv_usec;
}

float endTime (long start_time_usec) {

   struct timeval end;

   gettimeofday (&end, nullptr);

   return ((float)(end.tv_sec * 1000000 + end.tv_usec - start_time_usec)) / 1000000.0f;
}
