/*
 * SortedList.hpp
 *
 *  Created on: 6 août 2012
 *      Author: manuel
 */

#ifndef SORTEDLIST_HPP_
#define SORTEDLIST_HPP_

#include "common.h"

class SortedList {
public:
	typedef IntSet Container;
	typedef Container::iterator iterator;
	typedef Container::const_iterator const_iterator;
public:
	SortedList(size_t const & = 0, size_t const & = 0);
	virtual ~SortedList();

	void set(size_t const & key, size_t const & value);
	void increment(size_t const & key);
	void decrement(size_t const & key);

	size_t get(size_t const & key) const;
	size_t min() const;
	size_t max() const;

	const_iterator begin() const;
	const_iterator end() const;

	size_t max_key() const;
	size_t max_value() const;

	void allocate(size_t const & max_key, size_t const & max_value);
private:
	Container _set;
	IntVector _size;
	std::vector<Container::iterator> _key_positions;
	std::vector<Container::iterator> _value_positions;

};

std::ostream & operator<<(std::ostream & stream, SortedList const &);

inline SortedList::SortedList(size_t const & max_key,
		size_t const & max_value) {
	allocate(max_key, max_value);
}

inline SortedList::~SortedList() {

}
inline void SortedList::allocate(size_t const & max_key,
		size_t const & max_value) {
	_set.clear();
	_size.assign(max_value, 0);
	_key_positions.assign(max_key, _set.end());
	_value_positions.assign(max_value, _set.end());
}
inline size_t SortedList::get(size_t const & key) const {
	return _key_positions[key] == _set.end() ? 0 : *_key_positions[key];
}
inline size_t SortedList::min() const {

	return _set.empty() ? 0 : *_set.begin();
}
inline size_t SortedList::max() const {
	return _set.empty() ? 0 : *_set.rbegin();
}

inline SortedList::const_iterator SortedList::begin() const {
	return _set.begin();
}
inline SortedList::const_iterator SortedList::end() const {
	return _set.end();
}
inline size_t SortedList::max_key() const {
	return _key_positions.size();
}
inline size_t SortedList::max_value() const {
	return _size.size();
}

inline void SortedList::increment(size_t const & key) {
	set(key, get(key) + 1);
}

inline void SortedList::decrement(size_t const & key) {
	set(key, get(key) - 1);
}
inline void SortedList::set(size_t const & key, size_t const & value) {
	size_t const old(get(key));
	if (old != value) {
		--_size[old];
		++_size[value];
		if (_value_positions[value] == _set.end()) {
			if (old < value)
				// temps constant si insertion juste aprés
				_value_positions[value] = _set.insert(_key_positions[old],
						value);
			else {
				bool optimizeInsertion(false);
				if (_value_positions[old] != _set.begin()) {
					size_t const before(*(_value_positions[old]--));
					if (before < value) {
						_value_positions[value] = _set.insert(
								_value_positions[old]--, value);
						optimizeInsertion = true;
					}
				}
				if (!optimizeInsertion)
					_value_positions[value] = _set.insert(value).first;
			}
		}

		if (_size[old] == 0) {
			_set.erase(_value_positions[old]);
			_value_positions[old] = _set.end();
		}
		_key_positions[key] = _value_positions[value];
	}
}

inline std::ostream & operator<<(std::ostream & stream,
		SortedList const & rhs) {
	for (size_t i(0); i < rhs.max_key(); ++i) {
		std::cout << std::setw(8) << i;
		std::cout << std::setw(8) << rhs.get(i) << "\n";
	}
	std::copy(rhs.begin(), rhs.end(),
			std::ostream_iterator<size_t>(stream, " "));
	stream << "\n";
	return stream;
}
#endif /* SORTEDLIST_HPP_ */
