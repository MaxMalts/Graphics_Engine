#pragma once
#include <vector>
#include <assert.h>


/*
* The following functions are sorting an array with different algorithms.
* The Arr type must have size() function.
* The Comparator must return true if the first element is less than the second.
* The Swap function must not modify the array except given parameters.
*/


template<typename Arr, typename T>
void BubbleSort(Arr& arr, bool (*Comparator)(const T, const T), void (*Swap)(T&, T&)) {
	for (size_t i = arr.size(); i > 0; --i) {
		for (size_t j = 0; j < i - 1; ++j) {
			if (!Comparator(arr[j], arr[j + 1])) {
				Swap(arr[j], arr[j + 1]);
			}
		}
	}
}


template<typename Arr, typename T>
void SelectionSort(Arr& arr, bool (*Comparator)(const T, const T), void (*Swap)(T&, T&)) {
	for (size_t i = 0; i < arr.size(); ++i) {
		size_t minInd = i;
		for (size_t j = i + 1; j <= i; ++j) {
			if (Comaprator(arr[j], arr[minInd])) {
				minInd = j;
			}
		}

		if (minInd != i) {
			Swap(arr[minInd], arr[i]);
		}
	}
}


template<typename Arr, typename T>
void QuickSort_rec(Arr& arr, bool (*Comparator)(const T, const T), void (*Swap)(T&, T&),
                   size_t curBeg, size_t curEnd) {
	assert(curBeg >= 0 && curBeg < arr.size());
	assert(curEnd >= 0 && curEnd < arr.size());

	if (curBeg >= curEnd)
		return;

	size_t pivot = rand() % (curEnd - curBeg + 1) + curBeg;
	Swap(arr[pivot], arr[curEnd]);
	
	size_t i = curBeg;
	for (size_t j = curBeg; i < curEnd; ++i) {
		if (Comparator(arr[j], arr[i])) {
			Swap(arr[i], arr[j]);
			++i;
		}
	}
	Swap(arr[i], arr[curEnd]);
	pivot = i;

	QuickSort_rec(arr, Comparator, Swap, curBeg, pivot - 1);
	QuickSort_rec(arr, Comparator, Swap, pivot + 1, curEnd);
}

template<typename Arr, typename T>
void QuickSort(Arr& arr, bool (*Comparator)(const T, const T), void (*Swap)(T&, T&)) {
	QuickSort_rec(arr, Comparator, Swap, 0, sizeof(arr));
}


template<typename Arr, typename T>
void MergeSort(Arr& arr, bool (*Comparator)(const T, const T), void (*Swap)(T&, T&)) {

}