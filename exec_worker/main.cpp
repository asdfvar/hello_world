#include <deque>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>
#include <iostream>
#include <string>
#include <chrono>

std::mutex print_mutex;
void thread_print (const std::string& message) {
   std::lock_guard<std::mutex> lock (print_mutex);
   std::cout << message << std::endl;
}

class Semaphore {
   public:
      explicit Semaphore (int initial_count = 0) : count_ (initial_count) {}

      void wait (int id) {
         std::unique_lock<std::mutex> lock (mutex_);
         thread_print (std::to_string (id) + ": Waiting with wait count = " + std::to_string (count_));
         cv_.wait (lock, [this] { return count_ > 0; });
         count_--;
      }

      void post () {
         std::unique_lock<std::mutex> lock(mutex_);
         count_++;
         cv_.notify_one ();
      }

   private:
      std::mutex mutex_;
      std::condition_variable cv_;
      int count_;
};

struct Data {
   int data; // pretend this is a pointer to the data
};

// Synchronization primatives between setters, getters, and executive
struct ExecutiveControl
{
   ExecutiveControl () :
      queueSizeLimit (20),
      execDataPoolHasElement (0),
      setterDataPoolHasElement (0),
      getterDataPoolHasElement (0),
      setter_time_ms (0),
      getter_time_ms (0) { }

   bool finish;
   Semaphore queueSizeLimit;
   Semaphore execDataPoolHasElement;
   Semaphore setterDataPoolHasElement;
   Semaphore getterDataPoolHasElement;
   std::mutex execPoolAccessLock;
   std::mutex setterPoolAccessLock;
   unsigned int setter_time_ms, getter_time_ms;
};

struct DataNode {
   Data data;
   int check;
   bool finish;
};

// Setter
void setter (
      std::queue<DataNode>& execDataPool,
      std::queue<DataNode>& setterDataPool,
      ExecutiveControl& exeControl)
{
   DataNode dataNode;
   dataNode.finish = false;

   while (!exeControl.finish && !dataNode.finish)
   {
      exeControl.execDataPoolHasElement.wait (__LINE__);
      {
         std::lock_guard (exeControl.execPoolAccessLock);

         dataNode = execDataPool.front (); execDataPool.pop ();
         exeControl.queueSizeLimit.post ();
      }

      auto start = std::chrono::high_resolution_clock::now ();

      // Emulate processing load
      std::this_thread::sleep_for (std::chrono::milliseconds (200));

      auto end = std::chrono::high_resolution_clock::now ();
      auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - start);
      std::cout << "Duration = " << duration.count () << std::endl;
      exeControl.setter_time_ms = static_cast<unsigned int> (duration.count ());
      std::cout << "Setter time = " << exeControl.setter_time_ms << std::endl;

      {
         std::lock_guard (exeControl.setterPoolAccessLock);
         setterDataPool.push (dataNode);
      }

      thread_print ("front check value = " + std::to_string (setterDataPool.front ().check));
      exeControl.setterDataPoolHasElement.post ();

      thread_print ("moved " + std::to_string (dataNode.check) + " from exec-queue to setter-queue. Finish = " + std::to_string (dataNode.finish));
   }
}

// Getter
void getter (
      std::queue<DataNode>& setterDataPool,
      std::queue<DataNode>& getterDataPool,
      ExecutiveControl& exeControl)
{
   DataNode dataNode;
   dataNode.finish = false;

   while (!exeControl.finish && !dataNode.finish)
   {
      // wait until the front of the dataPool is set
      exeControl.setterDataPoolHasElement.wait (__LINE__);
      {
         std::lock_guard (exeControl.setterPoolAccessLock);

         // Get the data node
         dataNode = setterDataPool.front (); setterDataPool.pop ();
      }

      auto start = std::chrono::high_resolution_clock::now ();
      // Emulate processing load
      std::this_thread::sleep_for (std::chrono::milliseconds (200));
      auto end = std::chrono::high_resolution_clock::now ();
      exeControl.getter_time_ms = static_cast<unsigned int> (std::chrono::duration_cast<std::chrono::milliseconds> (end - start).count ());

      thread_print ("set data pool for index " + std::to_string (dataNode.check));

      getterDataPool.push (dataNode);
      exeControl.getterDataPoolHasElement.post ();
   }
}

int main ()
{
   std::queue<DataNode> execDataPool;
   std::queue<DataNode> setterDataPool;
   std::queue<DataNode> getterDataPool;

   std::vector<std::thread> setters;
   std::vector<std::thread> getters;

   const int num_setters = 3;
   const int num_getters = 2;
   const int numReads = 40;

   ExecutiveControl exeControl;
   exeControl.finish = false;

   for (int ind = 0; ind < num_setters; ind++)
      setters.push_back (
            std::thread (
               setter,
               std::ref (execDataPool),
               std::ref (setterDataPool),
               std::ref (exeControl)));

   for (int ind = 0; ind < num_getters; ind++)
      getters.push_back (
            std::thread (
               getter,
               std::ref (setterDataPool),
               std::ref (getterDataPool),
               std::ref (exeControl)));

   // Executive
   {
      for (int read = 0; read < numReads; read++)
      {
         thread_print ("reading index " + std::to_string (read));

         // TODO: make these variables thread safe
         // Check the timing
         if (exeControl.setter_time_ms / static_cast<float> (setters.size ()) > 10)
         {
            std::lock_guard (exeControl.execPoolAccessLock);
            setters.push_back (
                  std::thread (
                     setter,
                     std::ref (execDataPool),
                     std::ref (setterDataPool),
                     std::ref (exeControl)));
            thread_print ("Increasing the number of setters because time to set per thread = " + std::to_string (exeControl.setter_time_ms));
         }

         // Add an item to the data queue
         exeControl.queueSizeLimit.wait (__LINE__);
         {
            std::lock_guard (exeControl.execPoolAccessLock);

            DataNode dataNode;
            dataNode.check = read;
            dataNode.finish = read >= numReads - setters.size() ? true : false;

            execDataPool.push (dataNode);
            exeControl.execDataPoolHasElement.post ();
         }
      }

      exeControl.finish = true;
   }

   for (std::thread& setterThread : setters) setterThread.join ();
   for (std::thread& getterThread : getters) getterThread.join ();

   return 0;
}
