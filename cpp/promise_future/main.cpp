#include <iostream>
#include <future>
#include <thread>
#include <chrono>

using namespace std;

// Reference:
// https://thispointer.com/c11-multithreading-part-8-stdfuture-stdpromise-and-returning-values-from-thread/
// https://thispointer.com/c11-multithreading-part-9-stdasync-tutorial-example/
// https://thispointer.com/c11-multithreading-part-10-packaged_task-example-and-tutorial/

// 1. promise & future

void initializer(promise<int> *promise_ptr)
{
  cout << "Inside a seperate thread" << endl;
  promise_ptr->set_value(42);
}

void runPromiseFuture()
{
  promise<int> promise_obj;
  future<int> future_obj = promise_obj.get_future();
  thread th(initializer, &promise_obj);
  cout << "Get value from main thread: " << future_obj.get() << endl;
  th.join();
}

// 2. async & future

// Calling std::async with function pointer as callback
string fetchDataFromDB(string recvd_data)
{
  this_thread::sleep_for(chrono::seconds(5));
  return "DB_" + recvd_data;
}

// Calling std::async with function object as callback
struct DataFetcher
{
  string operator()(string recvd_data)
  {
    this_thread::sleep_for(chrono::seconds(5));
    return "DB_" + recvd_data;
  }
};

string fetchDataFromFile(string recvd_data)
{
  this_thread::sleep_for(chrono::seconds(5));
  return "File_" + recvd_data;
}

void runAsyncFuture()
{
  chrono::system_clock::time_point start = chrono::system_clock::now();

  // It automatically create a thread and a promise object for us,
  // then pass the promise object to threda function and return the
  // associated future object to us

  future<string> db_result = async(launch::async, fetchDataFromDB, "Data");

  // future<string> db_result = async(DataFetcher(), "Data");

  // Calling std::async with lambda function as callback
  // future<string> db_result = async([](string recvd_data) {
  //   this_thread::sleep_for(chrono::seconds(5));
  //   return "DB_" + recvd_data;
  // }, "Data");

  string file_data = fetchDataFromFile("Data");

  string db_data = db_result.get();

  auto end = chrono::system_clock::now();

  auto diff = chrono::duration_cast<chrono::seconds>(end - start).count();
  cout << "Total time taken: " << diff << " seconds" << endl;

  string final_data = db_data + "::" + file_data;

  cout << "Data: " << final_data << endl;

} 

// 3. packaged_task & future
void runPackagedFuture()
{
  packaged_task<string (string)> task(fetchDataFromDB);

  // packaged_task<string (string)> task(move(DataFetcher()));

  // packaged_task<string (string)> task([](string recvd_data){
  //   this_thread::sleep_for(chrono::seconds(5));
  //   return "DB_" + recvd_data;
  // });

  future<string> db_result = task.get_future();

  // because packaged_task is movable but not copyable
  thread th(move(task), "Data");

  cout << "Main thread get data from another thread: " << db_result.get() << endl;

  th.join();
}


int main(int argc, char** argv)
{
  runPromiseFuture();
  runAsyncFuture();
  runPackagedFuture();
  return 0;
}