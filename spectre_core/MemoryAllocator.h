#pragma once

#include <unordered_map>
#include <vector>

#include "FreeListAllocator.h"

class MemoryAllocator
{
private:

    struct FSA
    {
        int numBlocks;
        int numFreeBlocks;
        int blockSize;
        int freeP;
        void* base;
    };

    std::unordered_map<int, std::vector<FSA>> fsaHeap1;
    std::unordered_map<int, std::vector<FSA>> fsaHeap2;

    void* ptrToReserved;
    int memInUse;

    const static int alloc_reservation_size = 100 * 1024 * 1024;
    const static int alloc_commit_size_coalesce_1 = 1 * 1024 * 1024;
    const static int alloc_commit_size_coalesce_2 = 10 * 1024 * 1024;

    const static int edge_size_for_fsa = 512;
    const static int edge_size_for_first_coalesce_heap = 10 * 1024;
    const static int edge_size_for_coalesce_heap = 10 * 1024 * 1024;

    const static int small_page_size = 4 * 1024;
    const static int big_page_size = 32 * 1024;

    std::vector<void*> pages4;
    std::vector<void*> pages32;

    std::vector<FreeListAllocator*> coalesceHeap1;
    std::vector<FreeListAllocator*> coalesceHeap2;

    std::vector<void*> largeHeap;

    void* alloc(FSA& fsa);

    FSA& findFsaByAddress(void* p);

    void free(FSA& fsa, void* p);

public:

    MemoryAllocator();

    virtual ~MemoryAllocator();

    virtual void init();

    virtual void destroy();

    virtual void* alloc(size_t size);

    virtual void free(void* p);
};
