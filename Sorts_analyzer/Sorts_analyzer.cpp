#include <iostream>
#include <vector>
#include "Sorts.h"


/*

//The following flags are passed as parameters to the functions which counts their call times. 
enum StatsFuncFlag {
	StatsFuncFlag_initCounter,      // When this flag is passed you should pass the address of the counter too.
	StatsFuncFlag_process,          // Default value. If the counter was initialized, it increases.
	StatsFuncFlag_disableCounter    // When this flag is passed, function won't count its calls anymore.
};


bool Comparator(const int arg1, const int arg2, StatsFuncFlag flag = StatsFuncFlag_process,
                long long* counterParam = nullptr) {

	static long long* counter = nullptr;

	if (flag == StatsFuncFlag_initCounter) {
		assert(counter != nullptr);
		*counter = 0;

	} else if (flag == StatsFuncFlag_process) {
		if (counter != nullptr) {
			++(*counter);
		}
		return arg1 < arg2;

	} else {
		assert(flag == StatsFuncFlag_disableCounter);
		counter = nullptr;
	}

	return true;
}


void Swap(int* arg1, int* arg2, StatsFuncFlag flag = StatsFuncFlag_process,
          long long* counterParam = nullptr) {

	static long long* counter = nullptr;

	if (flag == StatsFuncFlag_initCounter) {
		assert(counter != nullptr);
		*counter = 1;

	} else if (flag == StatsFuncFlag_process) {
		assert(arg1 != nullptr);
		assert(arg2 != nullptr);

		if (counter != nullptr) {
			++(*counter);
		}

		int temp = *arg1;
		*arg1 = *arg2;
		*arg2 = temp;

	} else {
		assert(flag == StatsFuncFlag_disableCounter);
		counter = nullptr;
	}
}*/


class StatsFuncs {
public:

	static bool Comparator(const int val1, const int val2) {
		++comparatorCount;
		return val1 < val2;
	}


	static long long ComparatorCount() {
		return comparatorCount;
	}


	static void ResetComparator() {
		comparatorCount = 0;
	}



	static void Swap(int& val1, int& val2) {
		++swapCount;

		int temp = val1;
		val1 = val2;
		val2 = temp;
	}


	static long long SwapCount() {
		return swapCount;
	}


	static void ResetSwap() {
		swapCount = 0;
	}

private:
	static long long comparatorCount;
	static long long swapCount;
};
long long StatsFuncs::comparatorCount = 0;
long long StatsFuncs::swapCount = 0;



void FillRandom(std::vector<int>& arr, size_t count) {
	arr.clear();
	
	for (size_t i = 0; i < count; ++i) {
		arr.push_back(rand());
	}
}


template<typename T>
void PrintTable(std::vector<T>& arr) {
	assert(!arr.empty());

	for (int i = 0; i < arr.size(); ++i) {
		std::cout << i << "    " << arr[i] << '\n';
	}
}


void AnalyseSorts(const int maxCount, const int step) {
	assert(maxCount > 0);
	assert(step > 0);

	std::vector<long long> comparesByCount(maxCount, 0);
	std::vector<long long> swapsByCount(maxCount, 0);

	std::vector<int> arr;
	for (int curCount = 0; curCount < maxCount; curCount += step) {
		FillRandom(arr, curCount);

		BubbleSort<std::vector<int>, int>(arr, StatsFuncs::Comparator, StatsFuncs::Swap);

		comparesByCount[curCount] = StatsFuncs::ComparatorCount();
		swapsByCount[curCount] = StatsFuncs::SwapCount();

		StatsFuncs::ResetComparator();
		StatsFuncs::ResetSwap();
	}

	std::cout << "Compares:\n";
	PrintTable(comparesByCount);

	std::cout << "\nSwaps:\n";
	PrintTable(swapsByCount);
}


int main() {
	AnalyseSorts(100, 1);
	
	return 0;
}