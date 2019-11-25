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

    class Iterator
    {
    public:

        Iterator(LLRBTree<K, V>& llrbTreeRef) : llrbTreeRef(llrbTreeRef)
        {
            currentNode = llrbTreeRef.root;
            iteratorRoot = llrbTreeRef.root;
            shouldGoDown = true;
        }

        const K& key() const;

        const V& get() const;

        void set(const V& value);

        void next();

        void prev();

        bool hasNext() const;

        bool hasPrev() const;

    private:

        LLRBTree<K, V>& llrbTreeRef;

        typename LLRBTree<K, V>::Node* currentNode;

        typename LLRBTree<K, V>::Node* iteratorRoot;

        bool shouldGoDown;
    };

    Iterator iterator();

    const Iterator iterator() const;

private:

    LLRBTree<K, V> llrbTree;

    int size;
};

#include "Dictionary.hpp"
