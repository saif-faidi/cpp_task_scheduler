#include "thread_pool.h"
#include <type_traits>


thread_pool::thread_pool(size_t number_of_threads)
{
    if (number_of_threads == 0)
    {
        number_of_threads = 1; // ensure at least one thread
    }

    for (size_t i = 0; i < number_of_threads; ++i)
    {
        _wrokers.emplace_back(&thread_pool::worker_loop, this); 
    }
}


void thread_pool::worker_loop() {
    while (true) {
        std::function<void()> task ; 
        {
             std::unique_lock<std::mutex> lock(_mutex);
             // wait till stop is requested or there is a task in the queue
            _cv.wait(lock, [&] {
                return _stop || !_tasks.empty();
            }) ; 
            // return when stop is requested or the queue is emplty 
            if (_stop && _tasks.empty()) {
                return; 
            }
            // get the first task from the queue 
            task  = std::move(_tasks.front());
            _tasks.pop();
        }
        task(); // execute the task
    }
}


thread_pool::~thread_pool() {
    {
        std::lock_guard<std::mutex> lock(_mutex);
        _stop = true;
    }
    _cv.notify_all(); // wake up all worker threads
    
    // wait for all threads to finish 
    for( auto& t: _wrokers){
        if(t.joinable()){
            t.join();
        }
    }
}



template<typename F>
auto thread_pool :: submit(F&& func) -> std::future<std::invoke_result_t<F>> {

    // get the return type of the function to create the task
    // create the task 
    //get future object to be returned later
    // add to the queue a new task ( use lambda ) 
    // notify one worker that a task is available
    // return the future 
    using result_t  = std::invoke_result_t<F>;
    
    //std::packaged_task<result_t ()> task(std::forward<F>(func));
    auto task = std::make_shared<std::packaged_task<result_t ()>>(std::forward<F>(func));
    
    std::future<result_t> future = task -> get_future();


    std::lock_guard<std::mutex> lock(_mutex); 
    {
        _tasks.emplace([task = std::move(task)] () mutable {
            (*task)();
        });
        
    }
    _cv.notify_one(); // notify one worker thread

    return future;
}


void apply_thread_pool_example() {

    // create a thread pool object 
    thread_pool pool(4);

    auto f1 =  [](){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return 10;
    };

    auto f2 = []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        return std::string("Hello from f2 pool");
    }; 

    auto f3 = []() {
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
        throw std::runtime_error("Exception from f3");
        return 10 ; 
    }; 

    auto future1 = pool.submit(f1);
    auto future2 = pool.submit(f2);
    auto future3 = pool.submit(f3);


    std::cout << "Result from f1: " << future1.get() << "\n";
    std::cout << "Result from f2 :" << future2.get() << "\n"; 


    try {
        std::cout << "Result from f3: " << future3.get() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Caught exception from f3: " << e.what() << "\n";
    }

}