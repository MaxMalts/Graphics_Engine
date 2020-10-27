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
		for (size_t j = i + 1; j < arr.size(); ++j)
			if (Comparator(arr[j], arr[minInd]))
				minInd = j;

		if (minInd != i)
			Swap(arr[minInd], arr[i]);
	}
}


template<typename Arr, typename T>
void QuickSort_rec(Arr& arr, bool (*Comparator)(const T, const T), void (*Swap)(T&, T&),
                   int curBeg, int curEnd) {

	if (curBeg >= curEnd)
		return;

	int pivot = rand() % (curEnd - curBeg + 1) + curBeg;
	Swap(arr[pivot], arr[curEnd]);
	
	int i = curBeg;
	for (int j = curBeg; j < curEnd; ++j) {
		if (Comparator(arr[j], arr[curEnd])) {
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
	if (arr.size() > 1)
		QuickSort_rec(arr, Comparator, Swap, 0, arr.size() - 1);
}



template<typename Arr, typename T>
void Merge(Arr& arr, bool (*Comparator)(const T, const T), void (*Swap)(T&, T&),
           int curBeg, int curEnd, int secondStart) {

	for (int i = 0; i < secondStart; ++i) {
		if (arr[i] > arr[secondStart]) {
			Swap(arr[i], arr[secondStart]);
			//int first = arr[secondStart];

			// move Y[0] to its correct position to maintain sorted
			// order of Y[]. Note: Y[1..n-1] is already sorted
			for (int j = secondStart + 1; j <= curEnd && Comparator(arr[j], arr[j - 1]); ++j) {
				Swap(arr[j - 1], arr[j]);
			}
		}
	}
}

template<typename Arr, typename T>
void MergeSort_rec(Arr& arr, bool (*Comparator)(const T, const T), void (*Swap)(T&, T&),
                   int curBeg, int curEnd) {

	if (curBeg >= curEnd)
		return;

	int middle = (curEnd + curBeg) / 2;

	MergeSort_rec(arr, Comparator, Swap, curBeg, middle);
	MergeSort_rec(arr, Comparator, Swap, middle + 1, curEnd);
	Merge(arr, Comparator, Swap, curBeg, curEnd, middle + 1);
}

template<typename Arr, typename T>
void MergeSort(Arr& arr, bool (*Comparator)(const T, const T), void (*Swap)(T&, T&)) {
	if (arr.size() > 1)
		MergeSort_rec(arr, Comparator, Swap, 0, arr.size() - 1);
}