#include "gmock/gmock.h"
#include "Heap.hpp"

bool greater(int a, int b) {
    return a > b;
}

class HeapTests : public ::testing::Test {
protected:
    Heap<int, greater> heap{};

    static void load(Heap<int, greater> &heap) {
        heap.add(10);
        heap.add(7);
        heap.add(3);
        heap.add(13);
        heap.add(16);
        heap.add(11);
    }
};

TEST_F(HeapTests, Add) {
    load(heap);
    heap.add(8);
    ASSERT_EQ(heap.first(), 16);
    ASSERT_EQ(heap.last(), 3);
    ASSERT_EQ(heap.size(), 7);
    heap.add(20);
    ASSERT_EQ(heap.first(), 20);
    ASSERT_EQ(heap.last(), 3);
    ASSERT_EQ(heap.size(), 8);
}

TEST_F(HeapTests, Remove) {
    load(heap);
    ASSERT_EQ(heap.remove(), 16);
    ASSERT_EQ(heap.size(), 5);
}

TEST_F(HeapTests, RemoveHandlesEmptyHeap) {
    EXPECT_THROW(heap.remove(), EmptyException);
}

TEST_F(HeapTests, First) {
    load(heap);
    ASSERT_EQ(heap.first(), 16);
}

TEST_F(HeapTests, FirstHandleEmptyHeap) {
    EXPECT_THROW(heap.first(), EmptyException);
}

TEST_F(HeapTests, Last) {
    load(heap);
    ASSERT_EQ(heap.last(), 3);
}

TEST_F(HeapTests, LastHandleEmptyHeap) {
    EXPECT_THROW(heap.last(), EmptyException);
}

TEST_F(HeapTests, Empty) {
    ASSERT_TRUE(heap.empty());
    load(heap);
    ASSERT_FALSE(heap.empty());
}

TEST_F(HeapTests, Size) {
    load(heap);
    ASSERT_EQ(heap.size(), 6);
}