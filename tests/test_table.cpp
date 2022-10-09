#include <gtest/gtest.h>
#include "../poker/include/table.h"

#define TABLE_INDEX(index) GetValue(table->operator[](index))
#define NEXT(index) *(table->operator[](index).next->val)

int GetValue(const Table<int>::Node& node) {
    return *(node.val);
}

class TableTest : public testing::Test {
protected:
    void SetUp(){
        table = new Table<int>(1);
        table->push_front(2);
        table->push_back(3);
    }

    void TearDown(){
        delete table;
    }

    Table<int>* table;
};


TEST_F(TableTest, TestPushFront) {
    table->push_front(100);
    EXPECT_EQ(TABLE_INDEX(1), 100);
    EXPECT_EQ(TABLE_INDEX(2), 2);
    EXPECT_EQ(table->size, 4);
}

TEST_F(TableTest, TestPushBack) {
    table->push_back(100);
    EXPECT_EQ(TABLE_INDEX(3), 100);
    EXPECT_EQ(table->size, 4);
}

TEST_F(TableTest, TestPush) {
    {
        table->push(200, 2);
        EXPECT_EQ(TABLE_INDEX(3), 200);
		EXPECT_EQ(table->size, 4);
    }
    {
        table->push(200, 1);
        EXPECT_EQ(TABLE_INDEX(2), 200);
		EXPECT_EQ(table->size, 5);
    }
}

TEST_F(TableTest, IsCircle) {
    EXPECT_EQ(TABLE_INDEX(0), TABLE_INDEX(3));
}

TEST_F(TableTest, ShiftTestOnce) {
	table->shift();
	EXPECT_EQ(TABLE_INDEX(0), 2);
}

TEST_F(TableTest, ShiftTestRound) {
	// Shift size times
	table->shift(table->size);
	EXPECT_EQ(TABLE_INDEX(0), 1);
}

TEST_F(TableTest, PopTestFront) {
    table->pop_front();
    EXPECT_EQ(TABLE_INDEX(0), 2);
    EXPECT_EQ(NEXT(1), 2);
    EXPECT_EQ(table->size, 2);
}

TEST_F(TableTest, PopTestBack) {
    table->pop_back();
    EXPECT_EQ(TABLE_INDEX(1), 2);
    EXPECT_EQ(NEXT(1), 1);
    EXPECT_EQ(table->size, 2);
}

TEST_F(TableTest, PopTestByPosition) {
    table->pop(1);
    EXPECT_EQ(TABLE_INDEX(1), 3);
    EXPECT_EQ(NEXT(1), 1);
    EXPECT_EQ(table->size, 2);
}

TEST_F(TableTest, PopTestByValue) {
    int* node = table->operator[](1).val;
    table->pop(node);   
    EXPECT_EQ(TABLE_INDEX(1), 3);
    EXPECT_EQ(NEXT(1), 1);
    EXPECT_EQ(table->size, 2);

}

TEST_F(TableTest, TestEmptyTable) {
    Table<int> table2;
    EXPECT_EQ(table2.first, nullptr);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}