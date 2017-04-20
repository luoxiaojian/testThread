#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <future>

const long long AMOUNT_TO_INCREMENT_BY = 5000000000LL;

long long NumberIterator(volatile long long number, long long count) {
	count += number;
	while(number<count) ++number;
	return number;
}

int main(int argc, char **argv) {
	if(argc!=2) {
		std::cerr<<"Wrong number of arg"<<std::endl;
		return 0;
	}

	int numberOfThreads = atoi(argv[1]);

	long long number = 0, amountToIncrementByPerThread = AMOUNT_TO_INCREMENT_BY / numberOfThreads;
	int amountLeftOver = static_cast<int>(AMOUNT_TO_INCREMENT_BY % numberOfThreads);

	char *data = (char*)malloc(64*numberOfThreads);
	for(int i=0; i<numberOfThreads; i++) {
		*((long long*)&data[i*64]) = 0LL;
	}

	std::vector<std::future<long long>> myTasks;
	myTasks.reserve(numberOfThreads);

	auto timeVal = std::chrono::steady_clock::now();

	for(int i=0; i<numberOfThreads; ++i)
		myTasks.emplace_back(std::async(std::launch::async, NumberIterator, 0, amountToIncrementByPerThread));

	for(auto &i: myTasks)
		number += i.get();

	number = NumberIterator(number, amountLeftOver);

	std::chrono::duration<double> timePassed = std::chrono::steady_clock::now()-timeVal;

	std::cout<<"Total should be "<<AMOUNT_TO_INCREMENT_BY<<", is "<<number<<std::endl;
	std::cout<<"Elapsed time: "<<timePassed.count()<<"s."<<std::endl;

	return 0;
}
