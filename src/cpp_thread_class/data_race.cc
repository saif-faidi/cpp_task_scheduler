#include "data_race.h"
#include <thread>
#include <vector>
#include <iostream>


void increment_gloabl_counter(){
    
    int counter = 0;
    uint8_t num_threads = 10;
    std::vector<std::thread> threads; 
    threads.reserve(num_threads);


    // push threads in a vector 
    for (size_t i = 0; i < num_threads; ++i) {
        std::thread th = std::thread([&counter]() {
            for (size_t j = 0; j < 100000; ++j) {
                ++counter;
            }
        });
        threads.push_back(std::move(th));
    }
    
    // join threads
    for (auto& th : threads) {
        th.join();
    }

    std::cout << "counter: " << counter << std::endl;
}

