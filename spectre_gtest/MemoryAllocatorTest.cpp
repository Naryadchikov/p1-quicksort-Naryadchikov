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

TEST_F(MemoryAllocatorTest, SimpleAllocation)
{
    void* pi = allocator.alloc(sizeof(int));

    ASSERT_NE(pi, nullptr);
    ASSERT_EQ((uintptr_t)pi % 8, 0);
}
