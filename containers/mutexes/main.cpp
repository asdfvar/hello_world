#include <iostream>
#include <mutex>
#include <list>
#include <vector>
#include <memory>

int main ()
{
   std::list<std::mutex> myQueueList;
   myQueueList.resize (10);

   std::vector<std::unique_ptr<std::mutex>> mutexes;

   mutexes.push_back (std::make_unique<std::mutex>());
   mutexes.push_back (std::make_unique<std::mutex>());
   mutexes.push_back (std::make_unique<std::mutex>());
   mutexes.push_back (std::make_unique<std::mutex>());
   mutexes.push_back (std::make_unique<std::mutex>());

   mutexes[3]->lock ();
   mutexes[3]->unlock ();

   return 0;
}
