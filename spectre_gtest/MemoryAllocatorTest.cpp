#include "pch.h"

#include <set>

#include "spectre_core/MemoryAllocator.h"

class MemoryAllocatorTest : public ::testing::Test
{
protected:

    MemoryAllocator allocator;

    void SetUp() override
    {
        allocator.init();
    }

    void TearDown() override
    {
        allocator.destroy();
    }
};

TEST_F(MemoryAllocatorTest, ZeroAllocation)
{
    void* p = allocator.alloc(0);

    ASSERT_EQ(p, nullptr);
}

TEST_F(MemoryAllocatorTest, SimpleIntAllocation)
{
    void* pi = allocator.alloc(sizeof(int));

    ASSERT_NE(pi, nullptr);
    ASSERT_EQ((uintptr_t)pi % 8, 0);
}

TEST_F(MemoryAllocatorTest, SimpleArrayForFirstCoalesceHeapAllocation)
{
    void* pa = allocator.alloc(200 * sizeof(int));

    ASSERT_NE(pa, nullptr);
    ASSERT_EQ((uintptr_t)pa % 8, 0);
}

TEST_F(MemoryAllocatorTest, SimpleArrayForSecondCoalesceHeapAllocation)
{
    void* pa = allocator.alloc(10000 * sizeof(int));

    ASSERT_NE(pa, nullptr);
    ASSERT_EQ((uintptr_t)pa % 8, 0);
}

TEST_F(MemoryAllocatorTest, SimpleAllocationForLargeHeap)
{
    void* pa = allocator.alloc(11 * 1024 * 1024);

    ASSERT_NE(pa, nullptr);
    ASSERT_EQ((uintptr_t)pa % 8, 0);
}
