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

    checkerSet.insert(iterator.get());

    while (iterator.hasNext())
    {
        iterator.next();
        checkerSet.insert(iterator.get());
    }

    EXPECT_TRUE(initialSet == checkerSet);
}

TEST(SearchTest, BackwardIteratorMovementTest)
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
        iterator.next();
    }

    checkerSet.insert(iterator.get());

    while (iterator.hasPrev())
    {
        iterator.prev();
        checkerSet.insert(iterator.get());
    }

    EXPECT_TRUE(initialSet == checkerSet);
}