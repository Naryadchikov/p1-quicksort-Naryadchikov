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

    class Iterator
    {
    public:

        Iterator(Array<T>& arr);

        const T& get() const;

        void set(const T& value);

        void insert(const T& value);

        void remove();

        void next();

        void prev();

        void toIndex(int index);

        bool hasNext() const;

        bool hasPrev() const;

    private:
        int index;

        Array<T>& arr;
    };

    Iterator iterator();

    const Iterator iterator() const;

private:

    T* array;

    int capacity;

    int size;

    const static int array_initial_size = 16;

    const static int array_size_multiplier = 2;

    void resize(int newCapacity);
};

#include "array.hpp"
