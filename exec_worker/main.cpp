#include <deque>
#include <thread>
#include <mutex>
#include <queue>
#include <functional>
#include <condition_variable>
#include <iostream>
#include <string>
#include <chrono>
#include <stdexcept>

std::mutex print_mutex;
void thread_print (const std::string& message) {
   std::lock_guard<std::mutex> lock (print_mutex);
   std::cout << message << std::endl;
}

enum class Selection {ProcessNode, FinishStage1, FinishStage2};

class Latch {
   public:

      Latch (unsigned int count) : count_ (count) { }
      Latch () : count_ (0) { }

      void operator++ (int) {
         std::unique_lock<std::mutex> lock (mutex_);
         count_++;
      }

      void operator-- (int) {
         std::unique_lock<std::mutex> lock (mutex_);
         count_--;
      }

      void wait () {
         std::unique_lock<std::mutex> lock (mutex_);
         condition_variable_.wait (lock, [this] {return count_ <= 0;});
      }

      unsigned int size ()
      {
         return count_;
      }

   private:

      std::mutex mutex_;
      unsigned int count_;
      std::condition_variable condition_variable_;
};

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

// Synchronization primatives between stage1s, stage2s, and executive
struct ExecutiveControl
{
   ExecutiveControl () :
      dataQueueSize (6),
      setter_time_ms (0),
      getter_time_ms (0) { }

   unsigned int dataQueueSize;
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
      DataPool (int size) : sizeLimit_ (size), queueSizeLimit (size), enforceSizeLimit (true) { }
      DataPool () : enforceSizeLimit (false) { }

      DataNode pop () noexcept
      {
         hasElement.wait ();
         std::lock_guard<std::mutex> local_lock (lock);
         DataNode dataNode = dataPool.front ();
         dataPool.pop ();
         if (enforceSizeLimit) queueSizeLimit.post ();
         latch--;
         return dataNode;
      }

      void operator<< (DataNode& dataNode) noexcept
      {
         if (enforceSizeLimit) queueSizeLimit.wait ();
         std::lock_guard<std::mutex> local_lock (lock);
         latch++;
         dataPool.push (dataNode);
         hasElement.post ();
      }

      DataNode front () noexcept
      {
         std::lock_guard<std::mutex> local_lock (lock);
         return dataPool.front ();
      }

      int sizeLimit ()
      {
         if (!enforceSizeLimit)
            throw std::runtime_error ("Error: attempting to access the sizeLimit for a queue with no defined limit");

         std::lock_guard<std::mutex> local_lock (lock);
         return sizeLimit_;
      }

      // Debugging functionality
      void print (std::string&& msg) {
         std::lock_guard<std::mutex> local_lock (lock);
         std::queue<DataNode> tempQueue = dataPool;
         while (!tempQueue.empty ()) {
            thread_print (std::to_string (__LINE__) + ": " + msg + " Check = " + std::to_string (tempQueue.front ().check));
            tempQueue.pop ();
         }
         thread_print (std::to_string (__LINE__) + ": " + msg);
      }

   private:
      std::mutex lock;
      std::queue<DataNode> dataPool;
      Semaphore queueSizeLimit;
      Latch latch;
      int sizeLimit_;
      Semaphore hasElement;
      bool enforceSizeLimit;
};

class Timer {
   public:
      Timer (unsigned int& count) : count_ (&count)
   {
      start_ = std::chrono::high_resolution_clock::now ();
   }

      ~Timer () {
         end_ = std::chrono::high_resolution_clock::now ();
         auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end_ - start_);
         *count_ = static_cast<unsigned int> (duration.count ());
      }

   private:
      unsigned int* count_;
      std::chrono::time_point<std::chrono::high_resolution_clock> start_, end_;
};

// Setter
void setter (
      DataPool& stage1DataPool,
      DataPool& stage2DataPool,
      ExecutiveControl& exeControl)
{
   DataNode dataNode;

   do {
      // Get the data node
      dataNode = stage1DataPool.pop ();

      if (dataNode.selection == Selection::FinishStage2)
      {
         stage2DataPool << dataNode;
      }
      else if (dataNode.selection == Selection::ProcessNode)
      {

         auto start = std::chrono::high_resolution_clock::now ();

         // Emulate processing load
         {
            Timer timer (exeControl.setter_time_ms);
            std::this_thread::sleep_for (std::chrono::milliseconds (200));
         }

         stage1DataPool.print ("setter_processNode");
         thread_print (std::to_string (__LINE__) + ": Check = " + std::to_string (dataNode.check));
         stage2DataPool << dataNode;
      }
   } while (dataNode.selection != Selection::FinishStage1);
}

// Getter
void getter (
      DataPool& stage2DataPool,
      DataPool& stage3DataPool,
      ExecutiveControl& exeControl)
{
   DataNode dataNode;

   do {
      dataNode = stage2DataPool.pop ();

      if (dataNode.selection == Selection::ProcessNode)
      {
         // Emulate processing load
         {
            Timer timer (exeControl.getter_time_ms);
            std::this_thread::sleep_for (std::chrono::milliseconds (200));
         }

         thread_print (std::to_string (__LINE__) + ": Check = " + std::to_string (dataNode.check));
         stage3DataPool << dataNode;
      }
   } while (dataNode.selection != Selection::FinishStage2);
}

int main ()
{
   DataPool stage1DataPool (6);
   DataPool stage2DataPool;
   DataPool stage3DataPool;

   std::vector<std::thread> stage1s;
   std::vector<std::thread> stage2s;

   const int num_setters = 1;
   const int num_getters = 1;
   const int numReads = 20;

   ExecutiveControl exeControl;

   for (int ind = 0; ind < num_setters; ind++)
      stage1s.push_back (
            std::thread (
               setter,
               std::ref (stage1DataPool),
               std::ref (stage2DataPool),
               std::ref (exeControl)));

   for (int ind = 0; ind < num_getters; ind++)
      stage2s.push_back (
            std::thread (
               getter,
               std::ref (stage2DataPool),
               std::ref (stage3DataPool),
               std::ref (exeControl)));

   // Executive
   {
      for (int read = 0; read < numReads; read++)
      {
         thread_print ("Reading index " + std::to_string (read));

         // Check the timing and increase the number of stage1s or stage2s appropriately
         if (exeControl.setter_time_ms / static_cast<float> (stage1s.size ()) > 10 && stage1s.size () < stage1DataPool.sizeLimit ())
         {
            stage1s.push_back (
                  std::thread (
                     setter,
                     std::ref (stage1DataPool),
                     std::ref (stage2DataPool),
                     std::ref (exeControl)));
         }

         if (exeControl.getter_time_ms / static_cast<float> (stage2s.size ()) > 10 && stage2s.size () < stage1DataPool.sizeLimit ())
         {
            stage2s.push_back (
                  std::thread (
                     getter,
                     std::ref (stage2DataPool),
                     std::ref (stage3DataPool),
                     std::ref (exeControl)));
         }

         // Add an item to the data queue
         DataNode dataNode;
         dataNode.check = read;
         dataNode.selection = Selection::ProcessNode;

         thread_print (std::to_string (__LINE__) + ": Check = " + std::to_string (dataNode.check));

         stage1DataPool << dataNode;
      }

      // Use all the results
      for (int read = 0; read < numReads; read++) stage3DataPool.pop ();

      // Shutdown

      thread_print (std::to_string (__LINE__) + ": stage1s.size () = " + std::to_string (stage1s.size ()));
      thread_print (std::to_string (__LINE__) + ": stage2s.size () = " + std::to_string (stage2s.size ()));

      for (int ind = 0; ind < stage2s.size (); ind++) {
         DataNode dataNode;
         dataNode.selection = Selection::FinishStage2;
         dataNode.check = -2;

         stage1DataPool << dataNode;
      }

      for (int ind = 0; ind < stage1s.size (); ind++) {
         DataNode dataNode;
         dataNode.selection = Selection::FinishStage1;
         dataNode.check = -1;

         stage1DataPool << dataNode;
      }
   }

   for (std::thread& stageThread : stage1s) stageThread.join ();
   for (std::thread& stageThread : stage2s) stageThread.join ();

   return 0;
}
