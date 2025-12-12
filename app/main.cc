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
#include"power.h"
#include"return_thread.h"
#include "data_race.h"
#include"thread_pool.h"

int main()
{
    //power_main_example(); 
    //ret_th_example();
    //create_thread_and_pass_to_func();
    // uncaught_exception(); // this will cause an UB. 
    //increment_gloabl_counter();
    apply_thread_pool_example(); 
    return 0;
}
