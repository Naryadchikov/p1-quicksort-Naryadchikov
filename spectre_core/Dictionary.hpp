#pragma once


template <typename K, typename V>
Dictionary<K, V>::Dictionary()
{
    size = 0;
}

template <typename K, typename V>
Dictionary<K, V>::~Dictionary()
{
}

template <typename K, typename V>
void Dictionary<K, V>::put(const K& key, const V& value)
{
    llrbTree.insert(key, value);
    ++size;
}

template <typename K, typename V>
void Dictionary<K, V>::remove(const K& key)
{
    llrbTree.remove(key);
    --size;
}

template <typename K, typename V>
bool Dictionary<K, V>::contains(const K& key)
{
    return llrbTree.search(key) != nullptr;
}

template <typename K, typename V>
const V& Dictionary<K, V>::operator[](const K& key) const
{
    V* elementPtr = llrbTree.search(key);

    if (elementPtr != nullptr)
    {
        return *elementPtr;
    }

    return V();
}

template <typename K, typename V>
V& Dictionary<K, V>::operator[](const K& key)
{
    V* elementPtr = llrbTree.search(key);

    if (elementPtr != nullptr)
    {
        return *elementPtr;
    }

    llrbTree.insert(key, V());

    return V();
}

template <typename K, typename V>
int Dictionary<K, V>::getSize() const
{
    return size;
}
