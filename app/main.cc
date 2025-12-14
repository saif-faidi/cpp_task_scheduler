#include <chrono>
#include <cmath>
#include <format>
#include <mutex>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include <vector>
// #include "lib.h"
// #include"power.h"
// #include"return_thread.h"
// #include "data_race.h"
// #include"thread_pool.h"
// #include"para_execution.h"

#include <algorithm>
#include <execution>
#include <future>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <shared_mutex>
#include <stdexcept>
#include <type_traits>


using namespace std::literals;
std::mutex mtx;

struct Test
{

    void execute()
    {
        spdlog::info("execute");
    }
};

Test *ptest = nullptr;


void process()
{
    if (!ptest) // if ptest !=nullptr -> exceute directly there is no need to lock and unlock 
    {   
        std::lock_guard<std::mutex> lock(mtx);
        {   // maybe another thread already locked and unlocked the mutex , we need to retest if ptest is still nullprtr
            if (!ptest)
            {
                ptest = new Test(); // memory allocation , 
                    //creation of test pointer
                    // allocation to ptest ( 3 steps in one)
            }
        }

    }
    ptest->execute();
}

int main()
{

    std::vector<std::jthread> jthreads;

    for (size_t i = 0; i < 5; ++i)
    {
        jthreads.push_back(std::jthread(process));
    }

    for (auto &th : jthreads)
    {
        th.join();
    }

    delete ptest;

    spdlog::info("Program Finished...\n");

    return 0;
}


//power_main_example();
//ret_th_example();
//create_thread_and_pass_to_func();
// uncaught_exception(); // this will cause an UB.
//increment_gloabl_counter();
//apply_thread_pool_example();