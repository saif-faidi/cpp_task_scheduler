#include "lib.h"
#include <chrono>
#include <format>
#include <optional>
#include <spdlog/spdlog.h>
#include <string>
#include <thread>
#include <cmath>
#include<vector>
#include<mutex>



class Power {
    uint8_t power_active;
    uint8_t power_inactive = 0; 

    public: 
        explicit Power(uint8_t power_active, uint8_t power_inactive) : power_active(power_active), power_inactive(power_inactive) {}
        explicit Power(uint8_t power_active) : power_active(power_active) {}
        friend double operator+(const Power& rhs, const Power& lhs) noexcept; 
};


double operator+(const Power& rhs, const Power& lhs) noexcept {
    //simulate heavy claculation
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return std::sqrt(
        (rhs.power_active + lhs.power_active)*2
        + (rhs.power_inactive + lhs.power_inactive)*2
    );
}


int main()
{
    spdlog::info("----- Welcome to CppTaskScheduler! -----"); 

    // launch summation of 2 vectors one by one vec1[i] + vec2[i] in parallel
    std::vector<Power> vec1 = { Power(1, 2), Power(3, 4), Power(5), Power(1, 2), Power(3, 4), Power(5)};
    std::vector<Power> vec2 = {Power(10, 20), Power(30, 40), Power(50), Power(10, 20), Power(30, 40), Power(50)};

    spdlog::info(Power(1) + Power(2)); 

    if (vec1.size() != vec2.size()) {
        spdlog::error("Sizes of vectors are not equal");
        return 1;
    }

    std::mutex mtx; 
    std::vector<double> results; 
    std::vector<std::thread> threads;
    for (size_t i = 0; i < vec1.size(); i++) {
        threads.emplace_back([&mtx, &results] (const Power& p1, const Power& p2) {
            //std::cout << p1 + p2 << std::endl;
            std::lock_guard<std::mutex> lock(mtx);
            {
                results.emplace_back(p1 + p2);
            }
            //spdlog::info(std::format("{} + {} = {}", p1, p2, (p1 + p2)));
        }, std::cref(vec1[i]), std::cref(vec2[i]));
    }


      // Join all threads
    for (auto& t : threads) {
        t.join();
    }
    
    //print results 
    for(const auto& result : results) {
        std::cout << result << std::endl;
    }

    return 0;
}
