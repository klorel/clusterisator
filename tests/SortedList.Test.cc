#include <gtest/gtest.h>
#include "../utils/SortedList.hpp"
#include <set>

void call_set(SortedList & list, size_t const & i, size_t const & j) {
	std::cout << "------------------------------------\n";
	std::cout << "list.set( " << i << " , " << j << " )\n";
	list.set(i, j);
	std::cout << list;
}

void my_test() {

	SortedList list(4, 10);
	call_set(list, 0, 1);
	call_set(list, 1, 1);
	call_set(list, 2, 1);
	call_set(list, 3, 1);
	call_set(list, 1, 2);
	call_set(list, 0, 3);
	call_set(list, 2, 3);
	call_set(list, 0, 9);
	call_set(list, 0, 8);
}

TEST(SortedList, set) {
	my_test();
}

/**
 * Uses the fact that an std::sort is sorted, to make sure the SortedList is also sorted
 */
void testSorted(const std::set<size_t>& expected, const SortedList& list) {
	SortedList::const_iterator itList = list.begin();
	std::set<size_t>::iterator itExpected = expected.begin();

	while (itList != list.end()) {
		EXPECT_EQ(*itExpected, *itList);
		itList++;
		itExpected++;
	}

	EXPECT_EQ(itExpected, expected.end());
}

TEST(SortedList, sorted) {
	SortedList list(4, 10);
	std::set<size_t> expected;

	testSorted(expected, list);

	list.set(0, 1);
	expected.insert(1);
	testSorted(expected, list);

	list.set(1, 1);
	testSorted(expected, list);

	list.set(2, 1);
	testSorted(expected, list);

	list.set(3, 1);
	testSorted(expected, list);

	list.set(1, 2);
	expected.insert(2);
	testSorted(expected, list);

	list.set(0, 3);
	expected.insert(3);
	testSorted(expected, list);

	list.set(2, 3);
	testSorted(expected, list);

	list.set(0, 9);
	expected.insert(9);
	testSorted(expected, list);

	list.set(0, 8);
	expected.erase(9);
	expected.insert(8);
	testSorted(expected, list);
}

TEST(SortedList, getset) {
	SortedList list(4, 10);
	EXPECT_EQ((size_t) 0, list.get(0));
	EXPECT_EQ((size_t) 0, list.get(1));
	EXPECT_EQ((size_t) 0, list.get(2));
	EXPECT_EQ((size_t) 0, list.get(3));

	list.set(0, 1);
	EXPECT_EQ((size_t) 1, list.get(0));
	EXPECT_EQ((size_t) 0, list.get(1));
	EXPECT_EQ((size_t) 0, list.get(2));
	EXPECT_EQ((size_t) 0, list.get(3));

	list.set(1, 1);
	EXPECT_EQ((size_t) 1, list.get(0));
	EXPECT_EQ((size_t) 1, list.get(1));
	EXPECT_EQ((size_t) 0, list.get(2));
	EXPECT_EQ((size_t) 0, list.get(3));

	list.set(2, 1);
	EXPECT_EQ((size_t) 1, list.get(0));
	EXPECT_EQ((size_t) 1, list.get(1));
	EXPECT_EQ((size_t) 1, list.get(2));
	EXPECT_EQ((size_t) 0, list.get(3));

	list.set(3, 1);
	EXPECT_EQ((size_t) 1, list.get(0));
	EXPECT_EQ((size_t) 1, list.get(1));
	EXPECT_EQ((size_t) 1, list.get(2));
	EXPECT_EQ((size_t) 1, list.get(3));

	list.set(1, 2);
	EXPECT_EQ((size_t) 1, list.get(0));
	EXPECT_EQ((size_t) 2, list.get(1));
	EXPECT_EQ((size_t) 1, list.get(2));
	EXPECT_EQ((size_t) 1, list.get(3));

	list.set(0, 3);
	EXPECT_EQ((size_t) 3, list.get(0));
	EXPECT_EQ((size_t) 2, list.get(1));
	EXPECT_EQ((size_t) 1, list.get(2));
	EXPECT_EQ((size_t) 1, list.get(3));

	list.set(2, 3);
	EXPECT_EQ((size_t) 3, list.get(0));
	EXPECT_EQ((size_t) 2, list.get(1));
	EXPECT_EQ((size_t) 3, list.get(2));
	EXPECT_EQ((size_t) 1, list.get(3));

	list.set(0, 9);
	EXPECT_EQ((size_t) 9, list.get(0));
	EXPECT_EQ((size_t) 2, list.get(1));
	EXPECT_EQ((size_t) 3, list.get(2));
	EXPECT_EQ((size_t) 1, list.get(3));

	list.set(0, 8);
	EXPECT_EQ((size_t) 8, list.get(0));
	EXPECT_EQ((size_t) 2, list.get(1));
	EXPECT_EQ((size_t) 3, list.get(2));
	EXPECT_EQ((size_t) 1, list.get(3));
}
