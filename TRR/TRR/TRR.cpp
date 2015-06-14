#include <iostream>
#include <functional>
#include <chrono>
#include <vector>
#include <algorithm>
#include <numeric>
#include "TRR.h"
using namespace std;

int main() 
{
	std::vector<unsigned int> durations_tail;
	for (int i = 0; i < 100; i++) {
		auto start = std::chrono::system_clock::now();
		std::cout << "TailCalls: " << TailCall(calc_calls_count_trr(10000)) << std::endl;
		auto stop = std::chrono::system_clock::now();
		durations_tail.push_back(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count());
		std::cout << "Duration: " << durations_tail.back() << std::endl << std::endl;
	}

	std::vector<unsigned int> durations_simple;
	for (int i = 0; i < 1; i++) {
		auto start = std::chrono::system_clock::now();
		std::cout << "SimpleCalls: " << calc_calls_count_simple(1000) << std::endl;
		auto stop = std::chrono::system_clock::now();
		durations_simple.push_back(std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count());
		std::cout << "Duration: " << durations_simple.back() << std::endl << std::endl;
	}

	std::cout << "TailCall mean duration " << static_cast<double>(std::accumulate(durations_tail.begin(), durations_tail.end(), 0)) / durations_tail.size() << std::endl;
	std::cout << "SimpleCall mean duration " << static_cast<double>(std::accumulate(durations_simple.begin(), durations_simple.end(), 0)) / durations_simple.size() << std::endl;

	std::cin.get();
	return 0;
}