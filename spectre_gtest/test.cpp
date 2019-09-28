#include "pch.h"

#include "spectre_core/spectre.h"

TEST(TestCaseName, TestName)
{
    std::vector<int> v = { 1, 7, 4, 9, 22, 3, 0, -30 };
    std::vector<int> v_test = { -30, 0, 1, 3, 4, 7, 9, 22 };

    Sort(v, [](int a, int b) { return a < b; });

    for (int i = 0; i < v.size(); i++)
    {
        EXPECT_TRUE(v[i] == v_test[i]);
    }
}