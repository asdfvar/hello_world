#include <deque>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>

class Semaphore {
   public:
      explicit Semaphore (int initial_count = 0) : count_(initial_count) {}

      void wait () {
         std::unique_lock<std::mutex> lock(mutex_);
         cv_.wait(lock, [this] { return count_ > 0; });
         count_--;
      }

      void post () {
         std::unique_lock<std::mutex> lock(mutex_);
         count_++;
         cv_.notify_one(); // Or notify_all() if multiple threads can proceed
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
      getterDataPoolHasElement (0) { }

   bool finish;
   Semaphore queueSizeLimit;
   Semaphore execDataPoolHasElement;
   Semaphore setterDataPoolHasElement;
   Semaphore getterDataPoolHasElement;
   std::mutex execPoolAccessLock;
   std::mutex setterPoolAccessLock;
};

struct DataNode {
   Data data;
};

// Setter
void setter (
      std::queue<DataNode>& execDataPool,
      std::queue<DataNode>& setterDataPool,
      ExecutiveControl& exeControl)
{
   while (!exeControl.finish)
   {
      exeControl.execDataPoolHasElement.wait ();
      {
         std::lock_guard (exeControl.execPoolAccessLock);

         // Set the node element

         exeControl.setterDataPoolHasElement.post ();
         exeControl.queueSizeLimit.post ();
      }
   }
}

// Getter
void getter (
      std::queue<DataNode>& setterDataPool,
      std::queue<DataNode>& getterDataPool,
      ExecutiveControl& exeControl)
{
   while (!exeControl.finish)
   {
      DataNode dataNode = setterDataPool.front ();

      // wait until the front of the dataPool is set
      exeControl.setterDataPoolHasElement.wait ();
      {
         std::lock_guard (exeControl.execPoolAccessLock);

         // Get the data node
         dataNode = setterDataPool.front ();
         setterDataPool.pop ();

         getterDataPool.push (dataNode);
         exeControl.getterDataPoolHasElement.post ();
      }

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
   const int numReads = 8;

   ExecutiveControl exeControl;
   exeControl.finish = false;

   for (int ind = 0; ind < num_setters; ind++)
      setters.push_back (
            std::thread (
               std::ref (setter),
               std::ref (execDataPool),
               std::ref (setterDataPool),
               std::ref (exeControl)));

   for (int ind = 0; ind < num_getters; ind++)
      getters.push_back (
            std::thread (
               std::ref (getter),
               std::ref (setterDataPool),
               std::ref (getterDataPool),
               std::ref (exeControl)));

   for (int read = 0; read < numReads; read++)
   {
      // Add an item to the data queue
      exeControl.queueSizeLimit.wait ();

      std::lock_guard (exeControl.execPoolAccessLock);

      DataNode dataNode;
      execDataPool.push (dataNode);
      exeControl.execDataPoolHasElement.post ();
   }

   exeControl.finish = true;

   for (std::thread& setterThread : setters) setterThread.join ();
   for (std::thread& getterThread : getters) getterThread.join ();

   return 0;
}
