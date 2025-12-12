#pragma once

#include<iostream>
#include<thread>
#include<queue>
#include<future>
#include<vector>
#include<functional>
#include<condition_variable>
#include<mutex>
#include <type_traits>
#include <stdexcept>

class thread_pool {

    public:
        explicit thread_pool(size_t number_of_threads = std::thread::hardware_concurrency());
        ~thread_pool();
        thread_pool(const thread_pool&) = delete;
        thread_pool& operator=(const thread_pool&) =delete; // retrun by reference because when we cal b=c , b should be returned by reference.
        template<typename F>
        auto submit(F&& func) -> std::future<std::invoke_result_t<F>>; // trailing return type.

    private:
        void worker_loop();
        std::vector<std::thread> _wrokers; // used to store the threads
        std::queue<std::function<void()>> _tasks; // used to store the submitted tasks
        std::condition_variable _cv; // use to manage workers sleeping 
        std::mutex _mutex;  // sync the access to the queue and the same time the _stop variable.
        bool _stop{false};  // used to stop the thread pool

}; 


void apply_thread_pool_example();