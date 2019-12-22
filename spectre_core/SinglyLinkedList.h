#pragma once

template <class T>
class SinglyLinkedList
{
public:
    struct Node
    {
        T data;
        Node* next;
    };

    Node* head;

    SinglyLinkedList();

    void insert(Node* previousNode, Node* newNode);

    void remove(Node* previousNode, Node* deleteNode);
};

#include "SinglyLinkedList.hpp"
