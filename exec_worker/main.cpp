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

enum class Selection {ProcessNode, FinishGetter, FinishSetter};

class Semaphore {
   public:
      explicit Semaphore (int initial_count = 0) : count_ (initial_count) {}

      void wait () {
         std::unique_lock<std::mutex> lock (mutex_);
         cv_.wait (lock, [this] { return count_ > 0; });
         count_--;
      }

      void post () {
         std::unique_lock<std::mutex> lock(mutex_);
         count_++;
         cv_.notify_one ();
      }

      int size () {
         std::unique_lock<std::mutex> lock(mutex_);
         return count_;
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
      queueSizeSem (6),
      dataQueueSize (6), // must be set to the same value as above
      execDataPoolHasElement (0),
      setterDataPoolHasElement (0),
      getterDataPoolHasElement (0),
      setter_time_ms (0),
      getter_time_ms (0) { }

   Semaphore queueSizeSem;
   unsigned int dataQueueSize;
   Semaphore execDataPoolHasElement;
   Semaphore setterDataPoolHasElement;
   Semaphore getterDataPoolHasElement;
   std::mutex execPoolAccessLock;
   std::mutex setterPoolAccessLock;
   std::mutex getterPoolAccessLock;
   unsigned int setter_time_ms, getter_time_ms;
};

struct DataNode {
   Data data;
   int check;
   bool finish;
   Selection selection;
};

class DataPool
{
   public:
      DataPool (int size) : queueSizeLimit (size), enforceSizeLimit (true) { }
      DataPool () : enforceSizeLimit (false) { }

      DataNode pop () {
         hasElement.wait ();
         std::lock_guard<std::mutex> local_lock (lock);
         DataNode dataNode = dataPool.front ();
         dataPool.pop ();
//         if (enforceSizeLimit) queueSizeLimit.post ();
         return dataNode;
      }

      void operator<< (DataNode dataNode) {
//         if (enforceSizeLimit) queueSizeLimit.wait ();
         std::lock_guard<std::mutex> local_lock (lock);
         dataPool.push (dataNode);
         hasElement.post ();
      }

   private:
      std::mutex lock;
      std::queue<DataNode> dataPool;
      Semaphore queueSizeLimit;
      Semaphore hasElement;
      bool enforceSizeLimit;
};

// Setter
void setter (
      std::queue<DataNode>& execDataPool,
      DataPool& execDataPool_new,
      std::queue<DataNode>& setterDataPool,
      ExecutiveControl& exeControl)
{
   DataNode dataNode;

   do {
      // Get the data node

#if 1
      exeControl.execDataPoolHasElement.wait ();
      {
         std::lock_guard (exeControl.execPoolAccessLock);
         dataNode = execDataPool.front (); execDataPool.pop ();
         exeControl.queueSizeSem.post ();
      }
#else
      dataNode = execDataPool_new.pop ();
#endif

      if (dataNode.selection == Selection::FinishGetter)
      {
         std::lock_guard (exeControl.setterPoolAccessLock);
         setterDataPool.push (dataNode);
         exeControl.setterDataPoolHasElement.post ();
      }
      else if (dataNode.selection == Selection::ProcessNode)
      {

         auto start = std::chrono::high_resolution_clock::now ();

         // Emulate processing load
         std::this_thread::sleep_for (std::chrono::milliseconds (200));

         auto end = std::chrono::high_resolution_clock::now ();
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - start);
         exeControl.setter_time_ms = static_cast<unsigned int> (duration.count ());

         {
            std::lock_guard (exeControl.setterPoolAccessLock);
            setterDataPool.push (dataNode);
            exeControl.setterDataPoolHasElement.post ();
         }

         thread_print ("front check value = " + std::to_string (setterDataPool.front ().check));

         thread_print ("moved " + std::to_string (dataNode.check) + " from exec-queue to setter-queue. Finish = " + std::to_string (dataNode.finish));
      }
   } while (dataNode.selection != Selection::FinishSetter);
}

// Getter
void getter (
      std::queue<DataNode>& setterDataPool,
      std::queue<DataNode>& getterDataPool,
      ExecutiveControl& exeControl)
{
   DataNode dataNode;

   do {
      // wait until the front of the dataPool is set
      exeControl.setterDataPoolHasElement.wait ();
      {
         std::lock_guard (exeControl.setterPoolAccessLock);

         // Get the data node
         dataNode = setterDataPool.front (); setterDataPool.pop ();
      }

      if (dataNode.selection == Selection::ProcessNode)
      {
         auto start = std::chrono::high_resolution_clock::now ();

         // Emulate processing load
         std::this_thread::sleep_for (std::chrono::milliseconds (200));

         auto end = std::chrono::high_resolution_clock::now ();
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - start);
         exeControl.getter_time_ms = static_cast<unsigned int> (duration.count ());

         thread_print (std::to_string (__LINE__) + ": Processing data node " + std::to_string (dataNode.check));

         {
            std::lock_guard (exeControl.getterPoolAccessLock);
            getterDataPool.push (dataNode);
            exeControl.getterDataPoolHasElement.post ();
         }
      }
   } while (dataNode.selection != Selection::FinishGetter);
}

int main ()
{
   std::queue<DataNode> execDataPool;
   DataPool execDataPool_new (6);
   std::queue<DataNode> setterDataPool;
   std::queue<DataNode> getterDataPool;

   std::vector<std::thread> setters;
   std::vector<std::thread> getters;

   const int num_setters = 1;
   const int num_getters = 1;
   const int numReads = 20;

   ExecutiveControl exeControl;

   for (int ind = 0; ind < num_setters; ind++)
      setters.push_back (
            std::thread (
               setter,
               std::ref (execDataPool),
               std::ref (execDataPool_new),
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
         // Check the timing and increase the number of setters or getters appropriately
thread_print (std::to_string (__LINE__) + ": setter_time_ms = " + std::to_string (exeControl.setter_time_ms));
         if (exeControl.setter_time_ms / static_cast<float> (setters.size ()) > 10 && setters.size () < exeControl.dataQueueSize)
         {
            setters.push_back (
                  std::thread (
                     setter,
                     std::ref (execDataPool),
                     std::ref (execDataPool_new),
                     std::ref (setterDataPool),
                     std::ref (exeControl)));

            thread_print ("Increasing the number of setters because time to set per thread = " + std::to_string (exeControl.setter_time_ms));
         }

thread_print (std::to_string (__LINE__) + ": getter_time_ms = " + std::to_string (exeControl.getter_time_ms));
         if (exeControl.getter_time_ms / static_cast<float> (getters.size ()) > 10 && getters.size () < exeControl.dataQueueSize)
         {
            getters.push_back (
                  std::thread (
                     getter,
                     std::ref (setterDataPool),
                     std::ref (getterDataPool),
                     std::ref (exeControl)));
            thread_print (std::to_string (__LINE__) + ": Increasing the number of getters because time to set per thread = " + std::to_string (exeControl.getter_time_ms));
         }

         // Add an item to the data queue
         exeControl.queueSizeSem.wait ();
         {
            DataNode dataNode;
            dataNode.check = read;
            dataNode.selection = Selection::ProcessNode;

            std::lock_guard (exeControl.execPoolAccessLock);
            execDataPool.push (dataNode);
            exeControl.execDataPoolHasElement.post ();
            execDataPool_new << dataNode;
         }
      }

      // Shutdown

      thread_print (std::to_string (__LINE__) + ": setters.size () = " + std::to_string (setters.size ()));
      thread_print (std::to_string (__LINE__) + ": getters.size () = " + std::to_string (getters.size ()));

      for (int ind = 0; ind < getters.size (); ind++) {
         exeControl.queueSizeSem.wait ();
         DataNode dataNode;
         dataNode.selection = Selection::FinishGetter;

         std::lock_guard (exeControl.execPoolAccessLock);
         execDataPool.push (dataNode);
         exeControl.execDataPoolHasElement.post ();
         execDataPool_new << dataNode;
      }

      for (int ind = 0; ind < setters.size (); ind++) {
         exeControl.queueSizeSem.wait ();
         DataNode dataNode;
         dataNode.selection = Selection::FinishSetter;

         std::lock_guard (exeControl.execPoolAccessLock);
         execDataPool.push (dataNode);
         exeControl.execDataPoolHasElement.post ();
         execDataPool_new << dataNode;
      }
   }

   for (std::thread& setterThread : setters) setterThread.join ();
   for (std::thread& getterThread : getters) getterThread.join ();

   return 0;
}
