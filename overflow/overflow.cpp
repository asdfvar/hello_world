#include<iostream>

int main() {

   unsigned long n = 1; // max is 4294967296;
   unsigned long k = 7;

   unsigned long p;

   p = n - k;

   std::cout << "1 - 7 = " << p << " > 10 " << (p > 10) << std::endl;

   return 0;
}
