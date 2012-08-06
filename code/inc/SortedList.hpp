/*
 * SortedList.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef SORTEDLIST_HPP_
#define SORTEDLIST_HPP_

#include "common.h"

//class SortedList {
//public:
//	typedef IntList::iterator iterator;
//	typedef IntList::const_iterator const_iterator;
//public:
//	SortedList(size_t const & = 0, size_t const & = 0);
//	virtual ~SortedList();
//
//	void increment(size_t const & key);
//	size_t get(size_t const & key) const;
//	size_t min() const;
//	size_t max() const;
//
//	void allocate(size_t const & max_key, size_t const & max_value);
//private:
//	IntList _list;
//	IntVector _size;
//	std::vector<IntList::iterator> _positions;
//
//};
//
//inline SortedList::SortedList(size_t const & max_key,
//		size_t const & max_value) {
//	allocate(max_key, max_value);
//}
//
//inline SortedList::~SortedList() {
//
//}
//inline void SortedList::allocate(size_t const & max_key,
//		size_t const & max_value) {
//	_list.clear();
//	_size.assign(max_value, 0);
//	_positions.assign(max_key, _list.end());
//}
//inline size_t SortedList::get(size_t const & key) const {
//	return _positions[key] == _list.end() ? 0 : *_positions[key];
//}
//inline size_t SortedList::min() const {
//	return _list.front();
//}
//inline size_t SortedList::max() const {
//	return _list.back();
//}
//inline void SortedList::set(size_t const & key, size_t const & value) {
//	size_t const old(get(key));
//	if (old != value) {
//		--_size[old];
//		++_size[value];
//		if (_positions[value] == _list.end()) {
//
//		}
//	}
//}
#endif /* SORTEDLIST_HPP_ */
