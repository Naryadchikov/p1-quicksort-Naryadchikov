#include "pch.h"

#include <iostream>
#include<windows.h>

#include "MemoryAllocator.h"


MemoryAllocator::MemoryAllocator()
{
    ptrToReserved = nullptr;
    memInUse = 0;

    for (int i = 0, j = 8; i < 3; i++, j *= 2)
    {
        fsaHeap[i] = FSA{ small_page_size / j, small_page_size / j, j, 0, nullptr, nullptr };
    }

    for (int i = 3, j = 64; i < 7; i++, j *= 2)
    {
        fsaHeap[i] = FSA{ big_page_size / j, big_page_size / j, j, 0, nullptr, nullptr };
    }

    coalesceHeap1.push_back(new FreeListAllocator(alloc_commit_size_coalesce_1));
    coalesceHeap2.push_back(new FreeListAllocator(alloc_commit_size_coalesce_2));
}

MemoryAllocator::~MemoryAllocator()
{
    destroy();
}

void MemoryAllocator::init()
{
    // Reserve the memory
    ptrToReserved = VirtualAlloc(NULL, alloc_reservation_size, MEM_RESERVE, PAGE_READWRITE);

    // Commit first pages
    for (int i = 0; i < 3; i++)
    {
        VirtualAlloc(((char*)ptrToReserved) + memInUse, small_page_size, MEM_COMMIT, PAGE_READWRITE);

        fsaHeap[i].base = ((char*)ptrToReserved) + memInUse;

        memInUse += small_page_size;
    }

    for (int i = 3; i < 7; i++)
    {
        VirtualAlloc(((char*)ptrToReserved) + memInUse, big_page_size, MEM_COMMIT, PAGE_READWRITE);

        fsaHeap[i].base = ((char*)ptrToReserved) + memInUse;

        memInUse += big_page_size;
    }

    coalesceHeap1[0]->init(VirtualAlloc(((char*)ptrToReserved) + memInUse, alloc_commit_size_coalesce_1, MEM_COMMIT,
                                        PAGE_READWRITE));
    memInUse += alloc_commit_size_coalesce_1;

    coalesceHeap2[0]->init(VirtualAlloc(((char*)ptrToReserved) + memInUse, alloc_commit_size_coalesce_2, MEM_COMMIT,
                                        PAGE_READWRITE));
    memInUse += alloc_commit_size_coalesce_2;
}

void MemoryAllocator::destroy()
{
    VirtualFree(ptrToReserved, 0, MEM_RELEASE);

    ptrToReserved = nullptr;
    memInUse = 0;

    // Free large heap
    for (auto ptr : largeHeap)
    {
        VirtualFree(ptr, 0, MEM_RELEASE);
    }
}

void* MemoryAllocator::alloc(size_t size)
{
    if (size == 0)
    {
        return nullptr;
    }

    // Allocate in FSA
    if (size <= edge_size_for_fsa)
    {
        int alignedSize = size <= 8 ? 8 : std::pow(2, std::ceil(std::log(size) / std::log(2)));
        int fsaIndex = std::log2(alignedSize) - 3;
        FSA& fsaToUse = fsaHeap[fsaIndex];

        while (fsaToUse.numBlocks == 0 && fsaToUse.nextFsa != nullptr)
        {
            fsaToUse = *fsaToUse.nextFsa;
        }

        if (fsaToUse.numFreeBlocks > 0)
        {
            return alloc(fsaToUse);
        }

        // If we did not find FSA with free space we create new one
        // creating new FSA with 32Kb page
        if (alignedSize > 32)
        {
            if (memInUse + big_page_size > alloc_reservation_size)
            {
                return nullptr;
            }

            VirtualAlloc(((char*)ptrToReserved) + memInUse, big_page_size, MEM_COMMIT, PAGE_READWRITE);

            FSA newFsa = FSA{
                big_page_size / alignedSize, big_page_size / alignedSize, alignedSize, 0,
                ((char*)ptrToReserved) + memInUse, nullptr
            };

            fsaToUse.nextFsa = &newFsa;

            memInUse += big_page_size;

            return alloc(*fsaToUse.nextFsa);
        }

        // creating new FSA with 4Kb page
        if (memInUse + small_page_size > alloc_reservation_size)
        {
            return nullptr;
        }

        VirtualAlloc(((char*)ptrToReserved) + memInUse, small_page_size, MEM_COMMIT, PAGE_READWRITE);

        FSA newFsa = FSA{
            small_page_size / alignedSize, small_page_size / alignedSize, alignedSize, 0,
            ((char*)ptrToReserved) + memInUse, nullptr
        };

        fsaToUse.nextFsa = &newFsa;

        memInUse += small_page_size;

        return alloc(*fsaToUse.nextFsa);
    }

    // Allocate in First Coalesce Heap
    if (size <= edge_size_for_first_coalesce_heap)
    {
        void* p = nullptr;
        int i = 0;

        while (p == nullptr && i < ((int)coalesceHeap1.size()))
        {
            p = coalesceHeap1[i]->allocate(size);
            ++i;
        }

        if (p != nullptr)
        {
            return p;
        }

        // creating new FreeListAllocator
        if (memInUse + alloc_commit_size_coalesce_1 > alloc_reservation_size)
        {
            return nullptr;
        }

        coalesceHeap1.push_back(new FreeListAllocator(alloc_commit_size_coalesce_1));

        coalesceHeap1[coalesceHeap1.size() - 1]->init(VirtualAlloc(((char*)ptrToReserved) + memInUse,
                                                                   alloc_commit_size_coalesce_1, MEM_COMMIT,
                                                                   PAGE_READWRITE));

        memInUse += alloc_commit_size_coalesce_1;

        return coalesceHeap1[coalesceHeap1.size() - 1]->allocate(size);
    }

    // Allocate in Second Coalesce Heap
    if (size < edge_size_for_coalesce_heap)
    {
        void* p = nullptr;
        int i = 0;

        while (p == nullptr && i < ((int)coalesceHeap2.size()))
        {
            p = coalesceHeap2[i]->allocate(size);
            ++i;
        }

        if (p != nullptr)
        {
            return p;
        }

        // creating new FreeListAllocator
        if (memInUse + alloc_commit_size_coalesce_2 > alloc_reservation_size)
        {
            return nullptr;
        }

        coalesceHeap2.push_back(new FreeListAllocator(alloc_commit_size_coalesce_2));

        coalesceHeap2[coalesceHeap2.size() - 1]->init(VirtualAlloc(((char*)ptrToReserved) + memInUse,
                                                                   alloc_commit_size_coalesce_2, MEM_COMMIT,
                                                                   PAGE_READWRITE));

        memInUse += alloc_commit_size_coalesce_2;

        return coalesceHeap2[coalesceHeap2.size() - 1]->allocate(size);
    }

    // Allocate in Large Heap if 10+ Mb are asked for allocation
    if (memInUse + size > alloc_reservation_size)
    {
        return nullptr;
    }

    void* p = VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);

    largeHeap.push_back(p);

    return p;
}

void* MemoryAllocator::alloc(FSA& fsa)
{
    void* res = ((char*)fsa.base) + fsa.freeP * fsa.blockSize;

    fsa.freeP = *((int*)res);
    --fsa.numFreeBlocks;

    return res;
}

void MemoryAllocator::free(void* p)
{
    FSA& fsaWithSpaceToFree = findFsaByAddress(p);

    // Check for dummy
    if (fsaWithSpaceToFree.base != nullptr)
    {
        free(fsaWithSpaceToFree, p);

        return;
    }

    // Free for Coalesce Heap 1
    for (auto allocator : coalesceHeap1)
    {
        if (allocator->isInRange(p))
        {
            allocator->free(p);

            return;
        }
    }

    // Free for Coalesce Heap 2
    for (auto allocator : coalesceHeap2)
    {
        if (allocator->isInRange(p))
        {
            allocator->free(p);

            return;
        }
    }

    // Free for Large Heap
    if (std::find(largeHeap.begin(), largeHeap.end(), p) != largeHeap.end())
    {
        VirtualFree(p, 0, MEM_RELEASE);
    }
}

MemoryAllocator::FSA& MemoryAllocator::findFsaByAddress(void* p)
{
    for (FSA& firstFsa : fsaHeap)
    {
        FSA* fsaPtr = &firstFsa;

        while (fsaPtr != nullptr)
        {
            if ((int)p >= (int)(fsaPtr->base) &&
                (int)p < (int)(fsaPtr->base) + (int)big_page_size)
            {
                return *fsaPtr;
            }

            fsaPtr = fsaPtr->nextFsa;
        }
    }

    FSA dummy = FSA{ 0, 0, 0, 0, nullptr, nullptr };

    return dummy;
}

void MemoryAllocator::free(FSA& fsa, void* p)
{
    *((int*)p) = fsa.freeP;
    fsa.freeP = (((char*)p) - ((char*)fsa.base)) / (fsa.blockSize / sizeof(char));
}
