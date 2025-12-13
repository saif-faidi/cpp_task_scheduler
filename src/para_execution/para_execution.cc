#include "para_execution.h"


static std::mt19937 mt; // random number generator


double accum(const double *cbegin, const double *cend)
{
    return std::accumulate(cbegin, cend, 0.0);
}

double add_parallel_async(const std::vector<double> &vec)
{
    // get the first const pointer to the vector elements
    const auto v0 = &vec[0];

    // get the size fo the vector element
    size_t n = vec.size();
    if (n % 4 != 0)
    {
        throw std::invalid_argument("vector size must be a multiple of 4");
    }

    // create 4  different async calls and get their future values
    std::future<double> f1 =
        std::async(std::launch::async, accum, v0, v0 + (n / 4));
    std::future<double> f2 =
        std::async(std::launch::async, accum, v0 + (n / 4), v0 + (n / 2));
    std::future<double> f3 =
        std::async(std::launch::async, accum, v0 + (n / 2), v0 + (3 * n / 4));
    std::future<double> f4 =
        std::async(std::launch::async, accum, v0 + (3 * n / 4), v0 + n);

    // call get on the future and return the sum of values

    return f1.get() + f2.get() + f3.get() + f4.get();
}


double add_parallel_task(const std::vector<double> &vec)
{
    // get the first const pointer to the vector elements
    const auto v0 = &vec[0];

    // get the size fo the vector element
    size_t n = vec.size();
    if (n % 4 != 0)
    {
        throw std::invalid_argument("vector size must be a multiple of 4");
    }

    // create 4 packaged tasks
    std::packaged_task<double(const double *, const double *)> ptask1{accum};
    std::packaged_task<double(const double *, const double *)> ptask2{accum};
    std::packaged_task<double(const double *, const double *)> ptask3{accum};
    std::packaged_task<double(const double *, const double *)> ptask4{accum};

    // get all future objects
    std::future<double> f1 = ptask1.get_future();
    std::future<double> f2 = ptask2.get_future();
    std::future<double> f3 = ptask3.get_future();
    std::future<double> f4 = ptask4.get_future();

    // create 4 threads to launch the packaged tasks
    std::thread t1(std::move(ptask1), v0, v0 + (n / 4));
    std::thread t2(std::move(ptask2), v0 + n / 4, v0 + (n / 2));
    std::thread t3(std::move(ptask3), v0 + (n / 2), v0 + (3 * n / 4));
    std::thread t4(std::move(ptask4), v0 + (3 * n / 4), v0 + n);

    // join the threads
    t1.join();
    t2.join();
    t3.join();
    t4.join();

    // get and return the result
    return f1.get() + f2.get() + f3.get() + f4.get();
}

void main_para_execution_example()
{

    std::uniform_real_distribution<double> dist(
        1,
        100); // distributed between 1 and 100
    // generate a vector with 10'000 random elements
    std::vector<double> vrand(10'000);
    std::generate(vrand.begin(), vrand.end(), [&dist] { return dist(mt); });

    //call the add_parallel_async to add the elements using parallelism
    std::cout << add_parallel_async(vrand) << std::endl;
    std::cout << add_parallel_task(vrand) << std::endl;
    // prin the result
}