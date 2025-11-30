#include "lib.h"
#include <iostream>
#include <mutex>
#include <spdlog/spdlog.h>

std::mutex mtx;

void check_fizz_buzz(const std::string &caller_name, const int& i)
{
    
    if (i % 3 == 0 && i % 5 == 0)
    {
        spdlog::info(std::format("{} says fizzbuzz", caller_name));
    }
    else if (i % 3 == 0)
    {
        spdlog::info(std::format("{} says fizz", caller_name));
    }
    else if (i % 5 == 0)
    {
        spdlog::info(std::format("{} says buzz", caller_name));
    }
    else
    {
        spdlog::info(std::format("{} says {}", caller_name, i));
    }
}


void th_fizzbuzz(const std::string &caller_name, int &i)
{
    while (i < 20)
    {
        mtx.lock();
        {
            i++;
            check_fizz_buzz(caller_name, i);
        }
        mtx.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}


