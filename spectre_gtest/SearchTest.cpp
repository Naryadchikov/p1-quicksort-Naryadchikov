#include "pch.h"

#include <set>

#include "spectre_core/Dictionary.h"

TEST(SearchTest, PutSampleTest)
{
    Dictionary<int, int> dictionary;

    dictionary.put(10, 10);

    EXPECT_EQ(dictionary.getSize(), 1);
}

TEST(SearchTest, ContainsSampleTest)
{
    Dictionary<int, int> dictionary;

    dictionary.put(10, 10);

    EXPECT_TRUE(dictionary.contains(10));
    EXPECT_TRUE(!dictionary.contains(11));
}

TEST(SearchTest, RemoveSampleTest)
{
    Dictionary<int, int> dictionary;

    dictionary.put(10, 10);
    dictionary.put(11, 11);
    dictionary.remove(10);

    EXPECT_EQ(dictionary.getSize(), 1);
}

TEST(SearchTest, RemoveRootElementTest)
{
    Dictionary<int, int> dictionary;

    dictionary.put(10, 10);
    dictionary.remove(10);

    EXPECT_TRUE(!dictionary.contains(10));
}

TEST(SearchTest, RemoveWrongElementHandleTest)
{
    Dictionary<int, int> dictionary;

    dictionary.put(10, 10);

    EXPECT_NO_THROW(dictionary.remove(11));
}

TEST(SearchTest, AccessOperatorSampleTest)
{
    Dictionary<int, int> dictionary;

    dictionary.put(11, 101);

    EXPECT_EQ(dictionary[11], 101);
}

TEST(SearchTest, AccessOperatorUnexcitedElementTest)
{
    Dictionary<int, int> dictionary;

    EXPECT_EQ(dictionary[10], 0);
    EXPECT_EQ(dictionary.getSize(), 0);
}

TEST(SearchTest, IteratorKeyAndGetSampleTest)
{
    Dictionary<int, int> dictionary;

    dictionary.put(1, 11);

    auto iterator = dictionary.iterator();

    EXPECT_EQ(iterator.key(), 1);
    EXPECT_EQ(iterator.get(), 11);
}

TEST(SearchTest, IteratorSetSampleTest)
{
    Dictionary<int, int> dictionary;

    dictionary.put(1, 11);

    auto iterator = dictionary.iterator();

    iterator.set(101);

    EXPECT_EQ(iterator.key(), 1);
    EXPECT_EQ(iterator.get(), 101);
}

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