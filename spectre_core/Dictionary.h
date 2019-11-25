#pragma once

#include "LLRBTree.h"


template <typename K, typename V>
class Dictionary final
{
public:

    Dictionary();

    ~Dictionary();

    void put(const K& key, const V& value);

    void remove(const K& key);

    bool contains(const K& key);

    const V& operator[](const K& key) const;

    V& operator[](const K& key);

    int getSize() const;

private:

    LLRBTree<K, V> llrbTree;

    int size;
};

#include "Dictionary.hpp"
