#include <gtest/gtest.h>
#include "../client/include/table.h"


TEST(TestTable, TestPush) {
    Table<int> table(1);
    table.push_front(2);
    table.push_back(3);
    EXPECT_EQ(*(table[2].val), 3);
}

TEST(TestTable, IsCircle) {
    Table<int> table(1);
    table.push_front(2);
    table.push_front(3);
    EXPECT_EQ(table[0].val, table[3].val);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}