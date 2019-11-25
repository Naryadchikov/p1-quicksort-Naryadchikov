#pragma once


template <typename K, typename V>
class LLRBTree final
{
public:

    V search(const K& key);

    void insert(const K& key, const V& value);

    void remove(const K& key);

private:

    const static bool RED = true;
    const static bool BLACK = false;

    class Node
    {
    public:

        K key;

        V val;

        Node* left = nullptr;

        Node* right = nullptr;

        bool color;

        Node(const K& key, const V& val);
    };

    Node* root = nullptr;

    Node* insert(Node* h, const K& key, const V& value);

    Node* rotateLeft(Node* h);

    Node* rotateRight(Node* h);

    Node* moveRedLeft(Node* h);

    Node* moveRedRight(Node* h);

    void flipColors(Node* h);

    bool isRed(Node* h);

    Node* fixUp(Node* h);

    Node* deleteMin(Node* h);

    Node* minNode(Node* h);

    Node* remove(Node* h, const K& key);
};

#include "LLRBTree.hpp"
