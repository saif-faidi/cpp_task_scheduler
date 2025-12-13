#pragma once 

#include<iostream>
#include <thread>
#include <future>
#include <vector>
#include <numeric>
#include <random>


double accum(const double* cbegin, const double* cend);

double add_parallel_async(const std::vector<double>& vec); 
double add_parallel_task(const std::vector<double>& vec);

void main_para_execution_example(); 