#pragma once

#include "SinglyLinkedList.h"

class FreeListAllocator
{
private:

    struct FreeHeader
    {
        int blockSize;
    };

    struct AllocationHeader
    {
        int blockSize;
        char padding;
    };

    int totalSize;

    int memInUse;

    const static int alignment = 8;

    void* startPtr = nullptr;

    typedef SinglyLinkedList<FreeHeader>::Node Node;

    SinglyLinkedList<FreeHeader> freeList;

    void coalescence(Node* prevBlock, Node* freeBlock);

    void findBest(int size, int& padding, Node*& previousNode,
                  Node*& foundNode);

    static int calculatePaddingWithHeader(int baseAddress, int headerSize);

public:

    FreeListAllocator(int totalSize);

    virtual ~FreeListAllocator();

    virtual void* allocate(int size);

    virtual void free(void* p);

    virtual void init(void* p);

    virtual void reset();

    virtual bool isInRange(void* p);

};
