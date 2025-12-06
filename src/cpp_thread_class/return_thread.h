#include <iostream>
#include <thread>

void hello();
std::thread func();
void ret_th_example(); 
void cretae_thread_and_pass_to_func(std::thread th); 


void pass_th_to_me(std::thread th);
void create_thread_and_pass_to_func();

void hello_except();
void uncaught_exception() ;