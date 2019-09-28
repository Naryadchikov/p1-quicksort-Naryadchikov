#pragma once

#include "spectre.h"

#define CUTOFF 14


template <typename T, typename COMPARATOR>
void sort(std::vector<T>& collection, COMPARATOR comparator)
{
    sort(collection, comparator, 0, collection.size() - 1);

    assert(isSorted(collection, comparator));
}

template <typename T, typename COMPARATOR>
void sort(std::vector<T>& collection, COMPARATOR comparator, int lo, int hi)
{
    // cutoff to insertion sort
    int n = hi - lo + 1;

    if (n <= CUTOFF)
    {
        insertionSort(collection, comparator, lo, hi);

        return;
    }

    // finding of median-of-three 
    int m = median3(collection, comparator, lo, lo + n / 2, hi);

    exch(collection, m, lo);

    int j = partition(collection, comparator, lo, hi);

    sort(collection, comparator, lo, j - 1);
    sort(collection, comparator, j + 1, hi);

    assert(isSorted(collection, comparator, lo, hi));
}

template <typename T, typename COMPARATOR>
void insertionSort(std::vector<T>& collection, COMPARATOR comparator, int lo, int hi)
{
    for (int i = lo; i <= hi; i++)
    {
        for (int j = i; j > lo && comparator(collection[j], collection[j - 1]); j--)
        {
            exch(collection, j, j - 1);
        }
    }
}

// return the index of the median element among collection[i], collection[j], and collection[k]
template <typename T, typename COMPARATOR>
int median3(std::vector<T>& collection, COMPARATOR comparator, int i, int j, int k)
{
    return (comparator(collection[i], collection[j])
                ? (comparator(collection[j], collection[k]) ? j : comparator(collection[i], collection[k]) ? k : i)
                : (comparator(collection[k], collection[j]) ? j : comparator(collection[k], collection[i]) ? k : i));
}

template <typename T, typename COMPARATOR>
int partition(std::vector<T>& collection, COMPARATOR comparator, int lo, int hi)
{
    int i = lo;
    int j = hi + 1;
    T v = collection[lo];

    while (true)
    {
        // find item on lo to swap
        while (comparator(collection[++i], v))
        {
            if (i == hi) break;
        }

        // find item on hi to swap
        while (comparator(v, collection[--j]))
        {
            if (j == lo) break;
        }

        // check if pointers cross
        if (i >= j) break;

        exch(collection, i, j);
    }

    // put partitioning item v at collection[j]
    exch(collection, lo, j);

    // now, collection[lo .. j-1] <= collection[j] <= collection[j+1 .. hi]
    return j;
}

template <typename T>
void exch(std::vector<T>& collection, int i, int j)
{
    T temp = collection[i];

    collection[i] = collection[j];
    collection[j] = temp;
}

template <typename T, typename COMPARATOR>
bool isSorted(std::vector<T>& collection, COMPARATOR comparator)
{
    return isSorted(collection, comparator, 0, collection.size() - 1);
}

template <typename T, typename COMPARATOR>
bool isSorted(std::vector<T>& collection, COMPARATOR comparator, int lo, int hi)
{
    for (int i = lo + 1; i <= hi; i++)
    {
        if (comparator(collection[i], collection[i - 1]))
        {
            return false;
        }
    }

    return true;
}
