#include "pch.h"

#include "spectre_core/spectre.h"


TEST(SortTest, SimpleExample_01)
{
    std::vector<int> v = {1, 7, 4, 9, 22, 3, 0, -30};
    std::vector<int> v_test = {-30, 0, 1, 3, 4, 7, 9, 22};

    sort(v, [](int a, int b) { return a < b; });

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_TRUE(v[i] == v_test[i]);
    }
}

TEST(SortTest, SimpleExample_02)
{
    std::vector<int> v = {1, 7, 4, 9, 22, 3, 0, -30, 20, 30, 50, 40, 100, -100, 59};
    std::vector<int> v_test = {-100, -30, 0, 1, 3, 4, 7, 9, 20, 22, 30, 40, 50, 59, 100};

    sort(v, [](int a, int b) { return a < b; });

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_TRUE(v[i] == v_test[i]);
    }
}
