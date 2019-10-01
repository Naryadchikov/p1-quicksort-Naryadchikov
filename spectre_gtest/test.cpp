#include "pch.h"

#include "spectre_core/spectre.h"

#include <algorithm>


TEST(SortTest, IntegerArrayExample_01)
{
    std::vector<int> v = {1, 7, 4, 9, 22, 3, 0, -30};
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a < b; };

    std::sort(v_ideal.begin(), v_ideal.end(), comparatorLambda);

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, IntegerArrayExample_02)
{
    std::vector<int> v = {1, 7, 4, 9, 22, 3, 0, -30, 20, 30, 50, 40, 100, -100, 59};
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a < b; };

    std::sort(v_ideal.begin(), v_ideal.end(), comparatorLambda);

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, InitiallySortedIntegerArray)
{
    std::vector<int> v = {-30, 0, 1, 3, 4, 7, 9, 22};
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a < b; };

    std::sort(v_ideal.begin(), v_ideal.end(), comparatorLambda);

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, InitiallyReverseSortedIntegerArray)
{
    std::vector<int> v = {22, 9, 7, 4, 3, 1, 0, -30, -55, -59, -100, -101, -111, -777, -1111, -1771};
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a < b; };

    std::sort(v_ideal.begin(), v_ideal.end(), comparatorLambda);

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, IntegerArrayWithSameValues)
{
    std::vector<int> v = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a < b; };

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, IntegerArrayWithAllTheSameValuesExceptFirst)
{
    std::vector<int> v = {11, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a < b; };

    std::sort(v_ideal.begin(), v_ideal.end(), comparatorLambda);

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, IntegerArrayWithAllTheSameValuesExceptLast)
{
    std::vector<int> v = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0};
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a < b; };

    std::sort(v_ideal.begin(), v_ideal.end(), comparatorLambda);

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, IntegerArrayWithOutOFOrderElement)
{
    std::vector<int> v = {
        -1771, -1111, -111, -101, -71, -59, 60, -30, -11, 0, 5, 30, 59, 111, 171, 1771, 17771, 177771
    };
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a < b; };

    std::sort(v_ideal.begin(), v_ideal.end(), comparatorLambda);

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, IntegerArrayDescOrderExample_01)
{
    std::vector<int> v = {1, 7, 4, 9, 22, 3, 0, -30};
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a > b; };

    std::sort(v_ideal.begin(), v_ideal.end(), comparatorLambda);

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, IntegerArrayDescOrderExample_02)
{
    std::vector<int> v = {1, 7, 4, 9, 22, 3, 0, -30, 20, 30, 50, 40, 100, -100, 59};
    auto v_ideal(v);
    const auto comparatorLambda = [](int a, int b) { return a > b; };

    std::sort(v_ideal.begin(), v_ideal.end(), comparatorLambda);

    sort(v, comparatorLambda);

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_EQ(v[i], v_ideal[i]);
    }
}

TEST(SortTest, EmptyArray)
{
    std::vector<int> v = {};

    sort(v, [](int a, int b) { return a < b; });

    EXPECT_TRUE(v.empty());
}

TEST(SortTest, IntegerArrayWithWithSingleValue)
{
    std::vector<int> v = {1};

    sort(v, [](int a, int b) { return a < b; });

    EXPECT_TRUE(v.size() == 1);
}
