#include <iostream>
#include <thread>
#include <chrono>
#include "return_thread.h"

void hello()
{
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "hello" << std::endl;
}



/** First example */
std::thread func()
{
    std::thread th(hello);
    return th; // moved with modern c++ ( the thread class disabled the copy constructor so even with an lvalue it's moved to the main thread)
    //return std::thread(hello); // this is easier for the compiler to call the copy elision constructor 
}

void ret_th_example()
{

    std::thread th = func();
    std::cout << "Returned thread id: " << th.get_id() << std::endl;
    // this thread is now responsible to join the created thread as it is moved to the main thread
    th.join();
}


/** second example */

void pass_th_to_me(std::thread th) {
    std::cout << "received thread id: " << th.get_id() << std::endl;
    // join the thread 
    th.join();
}

void create_thread_and_pass_to_func() {
    std::thread th(hello);
    std::cout << "Created thread id: " << th.get_id() << std::endl;
    //pass_th_to_me(th); // Error: call to deleted constructor because copy constructor is disabled -> 
    //⭐ The compiler ONLY applies implicit move on return statements, NOT on normal arguments.
    pass_th_to_me(std::move(th)); // move the thread to the pass_to_me function
    // callin temporary object to move the thread to the pass_to_me function does not require std::move 
    // pass_th_to_me(std::thread(hello)); 

}



/** Example 3  */

void hello_except(){
    std::cout << "started hello_except" << std::endl;
    throw std::exception();
    std::cout << "ended hello_except" << std::endl; // this will not be executed 
}


void uncaught_exception() {
    try {
        std::thread th(hello_except);
        std::cout << "Created thread id: " << th.get_id() << std::endl;
        th.join();
    } catch(const std::exception& e ) { // This exception will never be caught because the try/catch is not in the same thread as the throw thread 
        std::cout << "Caught exception: " << e.what() << std::endl;
    }
    std::cout << "Finished..." << std::endl;
    // to resolve this the try / catch must be managed in the function called by the thread
}