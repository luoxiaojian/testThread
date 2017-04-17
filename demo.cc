#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>

const long long AMOUNT_TO_INCREMENT_BY = 5000000000LL;

void NumberIterator(long long *number, long long count) {
	long long localNumber = *number;
	count += localNumber;
	while(localNumber<count) ++localNumber;
	*number = localNumber;
}

int main(int argc, char **argv) {
	if(argc!=2) {
		std::cerr<<"Wrong number of arg"<<std::endl;
		return 0;
	}

	int numberOfThreads = atoi(argv[1]);

	long long number = 0, amountToIncrementByThread = AMOUNT_TO_INCREMENT_BY / numberOfThreads;
	int amountLeftOver = static_cast<int>(AMOUNT_TO_INCREMENT_BY % numberOfThreads);

	char *data = (char*)malloc(64*numberOfThreads);
	for(int i=0; i<numberOfThreads; i++) {
		*((long long*)&data[i*64]) = 0LL;
	}

	std::vector<std::thread> myThreads;
	myThreads.reserve(numberOfThreads);

	auto timeVal = std::chrono::steady_clock::now();

	for(int i=0; i<numberOfThreads; ++i)
		myThreads.push_back(std::thread(NumberIterator, (long long*)&data[i*64], amountToIncrementByPerThread));

	for(auto &i: myThreads)
		i.join();

	for(int i=0; i<numberOfThreads; i++) {
		number += *((long long*)&data[i*64]);
	}

	NumberIterator(&number, amountLeftOver);

	std::chrono::duration<double> timePassed = std::chrono::steady_clock::now()-timeVal;

	std::cout<<"Total should be "<<AMOUNT_TO_INCREMENT_BY<<", is "<<number<<std::endl;
	std::count<<"Elapsed time: "<<timePassed.count()<<"s."<<endl;

	return 0;
}
