#include "pch.h"

#include<windows.h>

#include "FreeListAllocator.h"


FreeListAllocator::FreeListAllocator(int totalSize)
{
    this->totalSize = totalSize;
    memInUse = 0;
}

void FreeListAllocator::init(void* p)
{
    startPtr = p;

    this->reset();
}

FreeListAllocator::~FreeListAllocator()
{
    startPtr = nullptr;
    totalSize = 0;
}

void* FreeListAllocator::allocate(int size)
{
    int allocationHeaderSize = sizeof(FreeListAllocator::AllocationHeader);

    // Search through the free list for a free block that has enough space to allocate our data
    int padding;
    Node* affectedNode;
    Node* previousNode;

    this->findBest(size, padding, previousNode, affectedNode);

    int alignmentPadding = padding - allocationHeaderSize;
    int requiredSize = size + padding;

    if (memInUse + requiredSize > totalSize)
    {
        return nullptr;
    }

    int rest = affectedNode->data.blockSize - requiredSize;

    if (rest > 0)
    {
        // We have to split the block into the data block and a free block of size 'rest'
        Node* newFreeNode = (Node*)((int)affectedNode + requiredSize);

        newFreeNode->data.blockSize = rest;
        freeList.insert(affectedNode, newFreeNode);
    }

    freeList.remove(previousNode, affectedNode);

    // Setup data block
    int headerAddress = (int)affectedNode + alignmentPadding;
    int dataAddress = headerAddress + allocationHeaderSize;

    ((FreeListAllocator::AllocationHeader*)headerAddress)->blockSize = requiredSize;
    ((FreeListAllocator::AllocationHeader*)headerAddress)->padding = alignmentPadding;

    memInUse += requiredSize;

    return (void*)dataAddress;
}

void FreeListAllocator::findBest(int size, int& padding, Node*& previousNode,
                                 Node*& foundNode)
{
    // Iterate whole list keeping a pointer to the best fit
    Node* bestBlock = nullptr;
    Node* it = freeList.head;
    Node* itPrev = nullptr;

    while (it != nullptr)
    {
        padding = calculatePaddingWithHeader((int)it, sizeof(FreeListAllocator::AllocationHeader));

        int requiredSpace = size + padding;

        if (it->data.blockSize >= requiredSpace)
        {
            bestBlock = it;
        }

        itPrev = it;
        it = it->next;
    }

    previousNode = itPrev;
    foundNode = bestBlock;
}

int FreeListAllocator::calculatePaddingWithHeader(int baseAddress, int headerSize)
{
    int multiplier = (baseAddress / alignment) + 1;
    int alignedAddress = multiplier * alignment;
    int padding = alignedAddress - baseAddress;
    int neededSpace = headerSize;

    if (padding < neededSpace)
    {
        // Header does not fit - Calculate next aligned address that header fits
        neededSpace -= padding;

        // How many alignments I need to fit the header        
        if (neededSpace % alignment > 0)
        {
            padding += alignment * (1 + (neededSpace / alignment));
        }
        else
        {
            padding += alignment * (neededSpace / alignment);
        }
    }

    return padding;
}

void FreeListAllocator::free(void* p)
{
    // Insert it in a sorted position by the address number
    int currentAddress = (int)p;
    int headerAddress = currentAddress - sizeof(FreeListAllocator::AllocationHeader);
    const FreeListAllocator::AllocationHeader* allocationHeader{(FreeListAllocator::AllocationHeader*)headerAddress};

    Node* freeNode = (Node*)(headerAddress);
    freeNode->data.blockSize = allocationHeader->blockSize + allocationHeader->padding;
    freeNode->next = nullptr;

    Node* it = freeList.head;
    Node* itPrev = nullptr;

    while (it != nullptr)
    {
        if (p < it)
        {
            freeList.insert(itPrev, freeNode);
            break;
        }

        itPrev = it;
        it = it->next;
    }

    memInUse -= freeNode->data.blockSize;

    // Merge contiguous nodes
    coalescence(itPrev, freeNode);
}

void FreeListAllocator::coalescence(Node* previousNode, Node* freeNode)
{
    if (freeNode->next != nullptr &&
        (int)freeNode + freeNode->data.blockSize == (int)freeNode->next)
    {
        freeNode->data.blockSize += freeNode->next->data.blockSize;
        freeList.remove(freeNode, freeNode->next);
    }

    if (previousNode != nullptr &&
        (int)previousNode + previousNode->data.blockSize == (int)freeNode)
    {
        previousNode->data.blockSize += freeNode->data.blockSize;
        freeList.remove(previousNode, freeNode);
    }
}

void FreeListAllocator::reset()
{
    memInUse = 0;

    Node* firstNode = (Node*)startPtr;

    firstNode->data.blockSize = totalSize;
    firstNode->next = nullptr;
    freeList.head = nullptr;
    freeList.insert(nullptr, firstNode);
}

bool FreeListAllocator::isInRange(void* p)
{
    return (uintptr_t)p >= (uintptr_t)startPtr &&
        (uintptr_t)p < (uintptr_t)startPtr + (uintptr_t)totalSize;
}
