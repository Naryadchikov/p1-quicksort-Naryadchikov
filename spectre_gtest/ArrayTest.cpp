#include "pch.h"

#include "spectre_core/array.h"

TEST(ArrayTest, DefaultConstructorInitTest)
{
    Array<int> array;

    EXPECT_EQ(array.getSize(), 0);
}

TEST(ArrayTest, CapacityConstructorInitTest)
{
    Array<int> array(10);

    EXPECT_EQ(array.getSize(), 0);
}

TEST(ArrayTest, AssignmentInitTest)
{
    Array<int> array;

    array.insert(1);
    array.insert(2);

    Array<int> newArray = array;

    EXPECT_EQ(newArray.getSize(), array.getSize());

    for (int i = 0; i < newArray.getSize(); i++)
    {
        EXPECT_EQ(newArray[i], array[i]);
    }
}

TEST(ArrayTest, SimpleInsertionTest)
{
    Array<int> array;

    array.insert(1);
    array.insert(5);
    array.insert(1, 2);
    array.insert(2, 3);
    array.insert(3, 4);

    EXPECT_EQ(array.getSize(), 5);

    for (int i = 0; i < array.getSize(); i++)
    {
        EXPECT_EQ(array[i], i + 1);
    }
}

TEST(ArrayTest, CapacityScalingTest)
{
    Array<int> array;
    const int testArrayActualSize = 100;

    for (int i = 0; i < testArrayActualSize; i++)
    {
        array.insert(i);
    }

    EXPECT_EQ(array.getSize(), testArrayActualSize);

    for (int i = 0; i < array.getSize(); i++)
    {
        EXPECT_EQ(array[i], i);
    }
}

TEST(ArrayTest, SimpleRemovingTest)
{
    Array<int> array;

    array.insert(1);
    array.insert(5);
    array.insert(1, 2);
    array.insert(2, 3);
    array.insert(3, 4);

    array.remove(0);

    EXPECT_EQ(array.getSize(), 4);

    for (int i = 0; i < array.getSize(); i++)
    {
        EXPECT_EQ(array[i], i + 2);
    }
}

TEST(ArrayTest, RemovingFromEmptyArrayThrowExceptionTest)
{
    Array<int> array;
    
    EXPECT_THROW(array.remove(0), std::out_of_range);
}

TEST(ArrayTest, RemoveElementWithWrongIndexThrowExceptionTest)
{
    Array<int> array;

    array.insert(1);

    EXPECT_THROW(array.remove(11), std::out_of_range);
}

TEST(ArrayTest, GettingElementFromEmptyArrayThrowExceptionTest)
{
    Array<int> array;

    EXPECT_THROW(array[0], std::out_of_range);
}

TEST(ArrayTest, GettingElementWithWrongIndexThrowExceptionTest)
{
    Array<int> array;

    array.insert(1);

    EXPECT_THROW(array[11], std::out_of_range);
}

TEST(ArrayTest, InsertingElementWithNegativeIndexThrowExceptionTest)
{
    Array<int> array;

    EXPECT_THROW(array.insert(-1, 1), std::out_of_range);
}

TEST(ArrayTest, InsertingElementWithBiggerThanSizeIndexThrowExceptionTest)
{
    Array<int> array;

    array.insert(0, 1);

    EXPECT_THROW(array.insert(2, 1), std::out_of_range);
}

TEST(ArrayTest, IteratorForwardMovingTest)
{
    Array<int> array;
    auto iterator = array.iterator();

    iterator.insert(0);
    iterator.insert(1);
    iterator.insert(2);
    iterator.insert(3);

    for (int i = 3; iterator.hasNext(); iterator.next(), i--)
    {
        EXPECT_EQ(iterator.get(), i);
    }
}

TEST(ArrayTest, IteratorBackwardMovingTest)
{
    Array<int> array;
    auto iterator = array.iterator();

    iterator.insert(0);
    iterator.insert(1);
    iterator.insert(2);
    iterator.insert(3);

    iterator.toIndex(3);

    for (int i = 0; iterator.hasPrev(); iterator.prev(), i++)
    {
        EXPECT_EQ(iterator.get(), i);
    }
}

TEST(ArrayTest, IteratorSettingAndRemovingTest)
{
    Array<int> array;
    auto iterator = array.iterator();

    array.insert(1);
    array.insert(1);
    array.insert(3);
    array.insert(10);
    array.insert(4);

    iterator.toIndex(3);
    iterator.remove();

    iterator.toIndex(1);
    iterator.set(2);

    iterator.toIndex(0);

    for (int i = 1; iterator.hasNext(); iterator.next(), i++)
    {
        EXPECT_EQ(iterator.get(), i);
    }
}
