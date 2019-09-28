#pragma once

#include "spectre.h"

template <typename T, typename COMPARATOR>
void Sort(std::vector<T>& Collection, COMPARATOR Comparator)
{
    int len = Collection.size();
    for (int i = 0; i < len; i++)
    {
        for (int j = 0; j < len; j++)
        {
            if (Comparator(Collection[i], Collection[j]))
            {
                T temp = Collection[j];
                Collection[j] = Collection[i];
                Collection[i] = temp;
            }
        }
    }
}
