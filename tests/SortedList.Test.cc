#include <gtest/gtest.h>
#include "SortedList.hpp"

void my_test() {

	SortedList list(4, 10);
	std::cout << list;
	list.set(0, 1);
	list.set(1, 1);
	list.set(2, 1);
	list.set(3, 1);
	std::cout << list;
	list.set(1, 2);
	std::cout << list;
	list.set(0, 3);
	std::cout << list;
	list.set(2, 3);
	std::cout << list;
	list.set(3, 3);
	std::cout << list;
	list.set(0, 9);
	std::cout << list;
	list.set(0, 8);
	std::cout << list;
}

TEST(SortedList, set) {
	my_test();
}
