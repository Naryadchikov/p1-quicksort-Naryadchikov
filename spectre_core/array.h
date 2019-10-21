#pragma once


template <typename T>
class Array final
{
public:

    Array();

    explicit Array(const int capacity);

    Array(const Array& a);

    Array(Array&& a) noexcept;

    ~Array();

    Array& operator=(const Array& a);

    Array& operator=(Array&& a) noexcept;

    void insert(const T& value);

    void insert(int index, const T& value);

    void remove(int index);

    const T& operator[](int index) const;

    T& operator[](int index);

    int getSize() const;

private:

    T* array;

    int capacity;

    int size;

    const static int array_initial_size = 16;

    const static int array_size_multiplier = 2;

    void resize(int newCapacity);
};

#include "array.hpp"
