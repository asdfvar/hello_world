#include <list>
#include <iostream>

class A
{
   public:
      A (int number)
      {
         value = number;
      }
      ~A (void)
      {
         std::cout << "deleted " << value << std::endl;
      }

   private:
      int value;
};

int main (void)
{
   std::list<A> As;

   for (int k = 0; k < 10; k++) As.push_back (k);

   As.clear();

   std::cout << "done clearing" << std::endl;
}
