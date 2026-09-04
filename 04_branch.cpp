#include<iostream>
#include<vector>
#include<algorithm>
#include<chrono>

// Set array size to 32000 numbers for branch predictability testing
const int ARRAY_SIZE=32000;

int main() {
	// Fill vector with pseudo-random numbers
	std::vector<int> data(ARRAY_SIZE);
	for(int i=0; i<ARRAY_SIZE; ++i) {
		data[i] = rand() % 256;
	}

	// TEST 1: UNSORTED DATA (UNPREDICTABLE BRANCHES)

	// Measure processing time when numbers are randomly ordered
	auto start1 = std::chrono::high_resolution_clock::now();
	long long sum1=0;

	for(int loop=0; loop<1000; ++loop) {
		for(int i=0; i<ARRAY_SIZE; ++i) {
			/* HARDWARE EXPLAINATION: Because data is random , this 'if' condition
			flips unpredictably between true and false. The CPU branch predictor
			constantly guesses wrong, triggering pipeline flushes and CPU stalls.*/
			if(data[i] >= 128) {
				sum1+=data[i];
			}
		}
	}
	auto end1 = std::chrono::high_resolution_clock::now();

	// TEST 2: SORTED DATA (PREDICTABLE BRANCHES)

	// Sort numbers so all small numbers come first, then large ones
	std::sort(data.begin(), data.end());

	// Measure processing time when condition path becomes stable.
	auto start2 = std::chrono::high_resolution_clock::now();
	long long sum2 = 0;

	for(int loop=0; loop<10000; ++loop) {
		for(int i=0; i<ARRAY_SIZE; ++i) {
			/* HARDWARE EXPLAINATION: The first ~half of elements are false,
			the rest are true. After 1-2 iterations, the CPU branch predictor
			learns the pattern, achieves near 100% prediction accuracy,
			and keeps the pipeline fully saturated.*/
			if(data[i] >= 128) {
				sum2+=data[i];
			}
		}
	}

	auto end2 = std::chrono::high_resolution_clock::now();

	std::cout << "Sum Check: " << sum1 + sum2 << "\n";

        std::cout << "Unsorted Data (Random Branches): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - start1).count() << " ms\n";

        std::cout << "Sorted Data (Predictable Branches): "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end2 - start2).count() << " ms\n";

        return 0;
}

