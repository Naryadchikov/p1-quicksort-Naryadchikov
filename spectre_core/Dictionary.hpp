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
    if (contains(key))
    {
        llrbTree.remove(key);
        --size;
    }
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

    return *new V();
}

template <typename K, typename V>
V& Dictionary<K, V>::operator[](const K& key)
{
    V* elementPtr = llrbTree.search(key);

    if (elementPtr != nullptr)
    {
        return *elementPtr;
    }

    V& defaultValueToBeInserted = *new V();

    llrbTree.insert(key, defaultValueToBeInserted);

    return defaultValueToBeInserted;
}

template <typename K, typename V>
int Dictionary<K, V>::getSize() const
{
    return size;
}

template <typename K, typename V>
Dictionary<K, V>::Iterator::Iterator(LLRBTree<K, V>& llrbTreeRef) : llrbTreeRef(llrbTreeRef)
{
    currentNode = llrbTreeRef.root;
    iteratorRoot = llrbTreeRef.root;

    lastNode = currentNode;

    while (lastNode->right != nullptr)
    {
        lastNode = lastNode->right;
    }

    if (lastNode->left != nullptr)
    {
        lastNode = lastNode->left;
    }

    shouldGoDown = true;
}

template <typename K, typename V>
const K& Dictionary<K, V>::Iterator::key() const
{
    return currentNode->key;
}

template <typename K, typename V>
const V& Dictionary<K, V>::Iterator::get() const
{
    return currentNode->val;
}

template <typename K, typename V>
void Dictionary<K, V>::Iterator::set(const V& value)
{
    currentNode->val = value;
}

template <typename K, typename V>
void Dictionary<K, V>::Iterator::next()
{
    if (shouldGoDown)
    {
        if (currentNode->left != nullptr)
        {
            currentNode = currentNode->left;
        }
        else
        {
            shouldGoDown = false;
        }
    }

    if (!shouldGoDown)
    {
        while (true)
        {
            typename LLRBTree<K, V>::Node* tmp = currentNode;

            if (currentNode == iteratorRoot)
            {
                break;
            }

            currentNode = currentNode->parent;

            if (currentNode->right != nullptr && currentNode->right != tmp)
            {
                break;
            }
        }

        if (currentNode == iteratorRoot)
        {
            iteratorRoot = currentNode->right;
        }

        currentNode = currentNode->right;

        shouldGoDown = true;
    }
}

template <typename K, typename V>
void Dictionary<K, V>::Iterator::prev()
{
    if (currentNode == currentNode->parent->left)
    {
        currentNode = currentNode->parent;
    }
    else
    {
        if (currentNode->parent->left == nullptr)
        {
            currentNode = currentNode->parent;
        }
        else
        {
            currentNode = currentNode->parent->left;

            while (currentNode->right != nullptr)
            {
                currentNode = currentNode->right;
            }
        }
    }
}

template <typename K, typename V>
bool Dictionary<K, V>::Iterator::hasNext() const
{
    return currentNode != lastNode;
}

template <typename K, typename V>
bool Dictionary<K, V>::Iterator::hasPrev() const
{
    return currentNode != llrbTreeRef.root;
}

template <typename K, typename V>
typename Dictionary<K, V>::Iterator Dictionary<K, V>::iterator()
{
    return *(new Iterator(this->llrbTree));
}

template <typename K, typename V>
const typename Dictionary<K, V>::Iterator Dictionary<K, V>::iterator() const
{
    return *(new Iterator(this->llrbTree));
}
