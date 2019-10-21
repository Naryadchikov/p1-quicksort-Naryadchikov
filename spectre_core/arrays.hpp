#pragma once

#include <stdexcept>


template <typename T>
Array<T>::Array() : capacity(array_initial_size), size(0)
{
    array = new T[capacity];
}

template <typename T>
Array<T>::Array(const int capacity) : capacity(capacity), size(0)
{
    array = new T[capacity];
}

template <typename T>
Array<T>::Array(const Array& a) : capacity(a.capacity), size(a.size)
{
    array = new T[capacity];

    for (int i = 0; i < size; i++)
    {
        array[i] = a.array[i];
    }
}

template <typename T>
Array<T>::Array(Array&& a) noexcept : capacity(a.capacity), size(a.size)
{
    array = a.array;
    a.array = nullptr;
}

template <typename T>
Array<T>::~Array()
{
    delete[] array;
}

template <typename T>
Array<T>& Array<T>::operator=(const Array& a)
{
    Array tmp(a);

    std::swap(array, tmp.array);
    std::swap(capacity, tmp.capacity);
    std::swap(size, tmp.size);

    return *this;
}

template <typename T>
Array<T>& Array<T>::operator=(Array&& a) noexcept
{
    std::swap(array, a.array);
    std::swap(capacity, a.capacity);
    std::swap(size, a.size);

    return *this;
}

template <typename T>
void Array<T>::insert(const T& value)
{
    if (size == capacity)
    {
        resize(capacity * array_size_multiplier);
    }

    size++;
    array[size - 1] = value;
}

template <typename T>
void Array<T>::insert(int index, const T& value)
{
    if (index < 0 || index > size)
    {
        throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
    }

    if (size == capacity)
    {
        resize(capacity * array_size_multiplier);
    }

    for (int i = size; i > index; i--)
    {
        array[i] = std::move(array[i - 1]);
    }

    array[index] = value;
    size++;
}

template <typename T>
void Array<T>::remove(int index)
{
    if (size == 0)
    {
        throw std::out_of_range("Array is empty, there is nothing to remove");
    }

    if (index < 0 || index > size - 1)
    {
        throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
    }

    for (int i = index; i < size - 1; i++)
    {
        array[i] = std::move(array[i + 1]);
    }

    size--;
}

template <typename T>
const T& Array<T>::operator[](int index) const
{
    if (size == 0)
    {
        throw std::out_of_range("Array is empty, there is nothing to get");
    }

    if (index < 0 || index > size - 1)
    {
        throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
    }

    return array[index];
}

template <typename T>
T& Array<T>::operator[](int index)
{
    if (size == 0)
    {
        throw std::out_of_range("Array is empty, there is nothing to get");
    }

    if (index < 0 || index > size - 1)
    {
        throw std::out_of_range("Index " + std::to_string(index) + " is out of range");
    }

    return array[index];
}

template <typename T>
int Array<T>::getSize() const
{
    return size;
}

template <typename T>
void Array<T>::resize(int newCapacity)
{
    Array tmp(newCapacity);

    for (int i = 0; i < size; i++)
    {
        tmp.array[i] = std::move(array[i]);
    }

    tmp.size = size;
    std::swap(*this, tmp);
}
