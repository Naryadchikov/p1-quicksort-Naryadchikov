#pragma once


template <typename K, typename V>
LLRBTree<K, V>::Node::Node(const K& key, const V& val)
{
    this->key = key;
    this->val = val;
    this->color = RED;
}

template <typename K, typename V>
V* LLRBTree<K, V>::search(const K& key)
{
    Node* x = root;

    while (x != nullptr)
    {
        if (key == x->key) 
        {
            return &(x->val);
        }
        else if (key < x->key)
        {
            x = x->left;
        }
        else if (key > x->key)
        {
            x = x->right;
        }
    }

    return nullptr;
}

template <typename K, typename V>
void LLRBTree<K, V>::insert(const K& key, const V& value)
{
    root = insert(root, key, value);
    root->color = BLACK;
}

template <typename K, typename V>
void LLRBTree<K, V>::remove(const K& key)
{
    root = remove(root, key);
    root->color = BLACK;
}

template <typename K, typename V>
typename LLRBTree<K, V>::Node* LLRBTree<K, V>::insert(Node* h, const K& key, const V& value)
{
    if (h == nullptr)
    {
        return new Node(key, value);
    }

    if (key == h->key)
    {
        h->val = value;
    }
    else if (key < h->key)
    {
        Node* newNode = insert(h->left, key, value);

        newNode->parent = h;
        h->left = newNode;
    }
    else
    {
        Node* newNode = insert(h->right, key, value);

        newNode->parent = h;
        h->right = newNode;
    }

    h = fixUp(h);

    return h;
}

template <typename K, typename V>
typename LLRBTree<K, V>::Node* LLRBTree<K, V>::rotateLeft(Node* h)
{
    Node* x = h->right;

    h->right = x->left;
    x->left = h;

    if (h->right != nullptr)
    {
        h->right->parent = h;
    }
    x->parent = h->parent;
    h->parent = x;

    x->color = h->color;
    h->color = RED;

    return x;
}

template <typename K, typename V>
typename LLRBTree<K, V>::Node* LLRBTree<K, V>::rotateRight(Node* h)
{
    Node* x = h->left;

    h->left = x->right;
    x->right = h;

    if (h->left != nullptr)
    {
        h->left->parent = h;
    }
    x->parent = h->parent;
    h->parent = x;

    x->color = h->color;
    h->color = RED;

    return x;
}

template <typename K, typename V>
typename LLRBTree<K, V>::Node* LLRBTree<K, V>::moveRedLeft(Node* h)
{
    flipColors(h);

    if (isRed(h->right->left)) 
    {
        h->right = rotateRight(h->right);
        h = rotateLeft(h);

        flipColors(h);
    }

    return h;
}

template <typename K, typename V>
typename LLRBTree<K, V>::Node* LLRBTree<K, V>::moveRedRight(Node* h)
{
    flipColors(h);

    if (isRed(h->left->left)) 
    {
        h = rotateRight(h);

        flipColors(h);
    }

    return h;
}

template <typename K, typename V>
void LLRBTree<K, V>::flipColors(Node* h)
{
    h->color = !h->color;
    h->left->color = !h->left->color;
    h->right->color = !h->right->color;
}

template <typename K, typename V>
bool LLRBTree<K, V>::isRed(Node* h)
{
    if (h == nullptr) 
    {
        return false;
    }

    return h->color == RED;
}

template <typename K, typename V>
typename LLRBTree<K, V>::Node* LLRBTree<K, V>::fixUp(Node* h)
{
    if (isRed(h->right) && !isRed(h->left))
    {
        h = rotateLeft(h);
    }

    if (isRed(h->left) && isRed(h->left->left))
    {
        h = rotateRight(h);
    }

    if (isRed(h->left) && isRed(h->right))
    {
        flipColors(h);
    }

    return h;
}

template <typename K, typename V>
typename LLRBTree<K, V>::Node* LLRBTree<K, V>::deleteMin(Node* h)
{
    if (h->left == nullptr)
    {
        return nullptr;
    }

    if (!isRed(h->left) && !isRed(h->left->left))
    {
        h = moveRedLeft(h);
    }

    h->left = deleteMin(h->left);

    return fixUp(h);
}

template <typename K, typename V>
typename LLRBTree<K, V>::Node* LLRBTree<K, V>::minNode(Node* h)
{
    return (h->left == nullptr) ? h : minNode(h->left);
}

template <typename K, typename V>
typename LLRBTree<K, V>::Node* LLRBTree<K, V>::remove(Node* h, const K& key)
{
    if (key < h->key) 
    {
        if (!isRed(h->left) && !isRed(h->left->left))
        {
            h = moveRedLeft(h);
        }

        h->left = remove(h->left, key);
    }
    else
    {
        if (isRed(h->left))
        {
            h = rotateRight(h);
        }

        if (key == h->key && h->right == nullptr)
        {
            return nullptr;
        }

        if (!isRed(h->right) && !isRed(h->right->left))
        {
            h = moveRedRight(h);
        }

        if (key == h->key) 
        {
            Node* mn = minNode(h->right);

            h->val = mn->val;
            h->key = mn->key;
            h->right = deleteMin(h->right);
        }
        else
        {
            h->right = remove(h->right, key);
        }
    }

    return fixUp(h);
}
