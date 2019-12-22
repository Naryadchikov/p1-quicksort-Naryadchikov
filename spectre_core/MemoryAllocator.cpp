#include "pch.h"

#include<windows.h>

#include "MemoryAllocator.h"


MemoryAllocator::MemoryAllocator()
{
    ptrToReserved = nullptr;
    memInUse = 0;

    for (int i = 8; i <= 32; i *= 2)
    {
        fsaHeap1[i] = std::vector<FSA>();
        fsaHeap1[i].push_back(FSA{small_page_size / i, small_page_size / i, i, 0, nullptr});
    }

    for (int i = 64; i <= 512; i *= 2)
    {
        fsaHeap2[i] = std::vector<FSA>();
        fsaHeap2[i].push_back(FSA{big_page_size / i, big_page_size / i, i, 0, nullptr});
    }
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
    for (int i = 0, j = 8; i < 3; i++, j *= 2)
    {
        pages4.push_back(VirtualAlloc(((char*)ptrToReserved) + memInUse, small_page_size, MEM_COMMIT, PAGE_READWRITE));

        fsaHeap1[j][0].base = ((char*)ptrToReserved) + memInUse;

        memInUse += small_page_size;
    }

    for (int i = 0, j = 64; i < 4; i++, j *= 2)
    {
        pages32.push_back(VirtualAlloc(((char*)ptrToReserved) + memInUse, big_page_size, MEM_COMMIT, PAGE_READWRITE));

        fsaHeap2[j][0].base = ((char*)ptrToReserved) + memInUse;

        memInUse += big_page_size;
    }
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
    // Allocate in FSA
    if (size <= edge_size_for_fsa)
    {
        int alignedSize = size <= 8 ? 8 : std::pow(2, std::ceil(std::log(size) / std::log(2)));
        std::vector<FSA> listOfFSAToUse = alignedSize > 32 ? fsaHeap2[alignedSize] : fsaHeap1[alignedSize];

        for (auto& fsaToUse : listOfFSAToUse)
        {
            if (fsaToUse.numFreeBlocks > 0)
            {
                return alloc(fsaToUse);
            }
        }

        // If we did not find FSA with free space we create new one
        // creating new FSA with 32Kb page
        if (alignedSize > 32)
        {
            pages32.push_back(
                VirtualAlloc(((char*)ptrToReserved) + memInUse, big_page_size, MEM_COMMIT, PAGE_READWRITE));

            fsaHeap2[alignedSize].push_back(FSA{
                big_page_size / alignedSize, big_page_size / alignedSize, alignedSize, 0,
                ((char*)ptrToReserved) + memInUse
            });

            memInUse += big_page_size;

            return alloc(fsaHeap2[alignedSize].back());
        }

        // creating new FSA with 4Kb page
        pages4.push_back(
            VirtualAlloc(((char*)ptrToReserved) + memInUse, small_page_size, MEM_COMMIT, PAGE_READWRITE));

        fsaHeap1[alignedSize].push_back(FSA{
            small_page_size / alignedSize, small_page_size / alignedSize, alignedSize, 0,
            ((char*)ptrToReserved) + memInUse
        });

        memInUse += small_page_size;

        return alloc(fsaHeap1[alignedSize].back());
    }

    // Allocate in First Coalesce Heap
    if (size <= edge_size_for_first_coalesce_heap)
    {
        return nullptr; // TODO: Implement Coalesce Heap
    }

    // Allocate in Second Coalesce Heap
    if (size < edge_size_for_coalesce_heap)
    {
        return nullptr; // TODO: Implement Coalesce Heap
    }

    // Allocate in Large Heap if 10+ Mb are asked for allocation
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

    // TODO: Free for Coalesce Heap

    // Free for Large Heap
    if (std::find(largeHeap.begin(), largeHeap.end(), p) != largeHeap.end())
    {
        VirtualFree(p, 0, MEM_RELEASE);
    }
}

MemoryAllocator::FSA& MemoryAllocator::findFsaByAddress(void* p)
{
    // Searching in the first FSA Heap
    for (auto& element : fsaHeap1)
    {
        for (FSA& fsa : element.second)
        {
            if ((uintptr_t)p >= (uintptr_t)fsa.base &&
                (uintptr_t)p < (uintptr_t)fsa.base + (uintptr_t)small_page_size)
            {
                return fsa;
            }
        }
    }

    // Searching in the second FSA Heap
    for (auto& element : fsaHeap2)
    {
        for (FSA& fsa : element.second)
        {
            if ((uintptr_t)p >= (uintptr_t)fsa.base &&
                (uintptr_t)p < (uintptr_t)fsa.base + (uintptr_t)big_page_size)
            {
                return fsa;
            }
        }
    }

    FSA dummy = FSA{0, 0, 0, 0, nullptr};

    return dummy;
}

void MemoryAllocator::free(FSA& fsa, void* p)
{
    *((int*)p) = fsa.freeP;
    fsa.freeP = (((char*)p) - ((char*)fsa.base)) / (fsa.blockSize / sizeof(char));
}
