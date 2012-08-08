#include <gtest/gtest.h>
#include "SortedList.hpp"

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
