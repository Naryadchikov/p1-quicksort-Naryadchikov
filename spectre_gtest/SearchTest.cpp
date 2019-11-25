#include "pch.h"

#include <set>

#include "spectre_core/Dictionary.h"

TEST(SearchTest, ForwardIteratorMovementTest)
{
    std::set<int> initialSet;
    std::set<int> checkerSet;
    Dictionary<int, int> dictionary;

    for (int i = 0; i < 15; i++)
    {
        dictionary.put(i, i);
        initialSet.insert(i);
    }

    auto iterator = dictionary.iterator();

    while (iterator.hasNext())
    {
        checkerSet.insert(iterator.get());
        iterator.next();
    }

    EXPECT_TRUE(initialSet == checkerSet);
}