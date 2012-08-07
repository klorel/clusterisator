#ifndef _INDEXED_LIST_HPP
#define _INDEXED_LIST_HPP

#include "common.h"

#include "Number.hpp"

/**
 * Behave like a specialized and optimized unordered_set.
 * Let add, remove, and test the existence of an element in O(1).
 * 
 * The counterparts of the optimizations are:
 * - It may only contains size_t
 * - Size isn't dynamic and we may only insert elements of [0, max_size[
 */
class IndexedList {
public:
	typedef IntVector::const_reverse_iterator const_iterator;
	typedef IntVector::reverse_iterator iterator;
	typedef IntVector::value_type value_type;
protected:
	/// liste courante des elements : dans un vector oui !!
	IntVector _list;
	/// vecteur des positions des elements dans _list
	IntVector _element;
public:
	IndexedList(size_t const & = 0);
	IndexedList(size_t const &, bool const & fill);
	IndexedList(IndexedList const&);

	IndexedList & operator=(IndexedList const &);

	size_t front() const;
	size_t random() const;
	size_t pop_random();
	size_t size() const;
	size_t max_size() const;

	void insert(size_t const&);
	void erase(size_t const&);

	void fill();
	bool contains(size_t const &) const;
	void clear();
	void reset(size_t const &);

	bool empty() const;

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;

	void check() const;

};

inline IndexedList::IndexedList(size_t const & nbClust) :
		_list(), _element(nbClust, nbClust) {
	_list.reserve(max_size());
}
inline IndexedList::IndexedList(size_t const & nbClust, bool const & doFill) :
		_list(), _element(nbClust, nbClust) {
	_list.reserve(nbClust);
	if (doFill) {
		fill();
	}
}

inline IndexedList::IndexedList(IndexedList const & orig) :
		_list(orig._list), _element(orig._element) {

}

inline void IndexedList::fill() {
	for (size_t i(0); i < _element.size(); ++i)
		insert(i);
}
inline void IndexedList::reset(size_t const & s) {
	_element.assign(s, s);
	_list.clear();
	_list.reserve(max_size());
}

inline size_t IndexedList::size() const {
	return _list.size();
}

inline size_t IndexedList::max_size() const {
	return _element.size();
}

inline void IndexedList::clear(void) {
	_list.clear();
	_element.assign(max_size(), max_size());
}

inline bool IndexedList::contains(size_t const &e) const {
	return _element[e] != max_size();
}
inline bool IndexedList::empty(void) const {
	return _list.empty();
}

inline void IndexedList::insert(size_t const&e) {
	/// si l'element n'est pas deja dans la liste
	if (!contains(e)) {
		//		DEBUG_TRACE("add "<<e<<std::endl);
		_element[e] = size();
		_list.push_back(e);
	}
	//	check();
	//	TRACE_N(size());
	//	TRACE_N(max_size());
	//	DEBUG_ASSERT("NO REALLOCATION ALLOWED"&&size()<=max_size());
}

inline void IndexedList::erase(size_t const&e) {

	//	check();
	if (contains(e)) {
		//		TRACE("remove "<<e<<" " <<_element[e]<<std::endl);
		//		for (IntVector::const_iterator i(_list.begin()); i != _list.end(); ++i)
		//			std::cout << *i << std::endl;
		/// on switch avec le dernier puis on réduit
		//		std::cout << "_list[" << _element[e] << "] = " << _list.back()
		//				<< std::endl;

		_list[_element[e]] = front();
		_element[front()] = _element[e];

		//		std::cout << "_element[" << e << "] = " << max_size() << std::endl;

		_element[e] = max_size();

		_list[size() - 1] = -1;

		//		std::cout << "_list[" << size() - 1 << "] = -1;" << std::endl;

		_list.erase(_list.begin() + _list.size() - 1);
		//		std::cout << "###########" << std::endl;
		//		for (IntVector::const_iterator i(_list.begin()); i != _list.end(); ++i)
		//			std::cout << *i << std::endl;
		///
	}
	//	check();
	//	TRACE_N(size());
	//	TRACE_N(max_size());
	//	DEBUG_ASSERT("NO REALLOCATION ALLOWED"&&size()<=max_size());
}

inline void IndexedList::check() const {
	for (size_t e(0); e < max_size(); ++e) {
		std::cout << e << " = " << _element[e] << std::endl;
		if (contains(e)) {
			assert(_list[_element[e]] == e);
		}
	}
}

inline IndexedList::iterator IndexedList::begin() {
	return _list.rbegin();
}

inline IndexedList::iterator IndexedList::end() {
	return _list.rend();
}

inline IndexedList::const_iterator IndexedList::begin() const {
	return _list.rbegin();
}

inline IndexedList::const_iterator IndexedList::end() const {
	return _list.rend();
}
inline size_t IndexedList::front() const {
	return _list.back();
}
inline size_t IndexedList::random() const {
	assert(size() > 0);
	return _list[Number::Generator() % size()];
}
inline size_t IndexedList::pop_random() {
	assert(size() > 0);
	size_t const e(_list[Number::Generator() % size()]);
	erase(e);
	return e;
}
inline IndexedList & IndexedList::operator=(IndexedList const &orig) {
	if (this != &orig) {
		_list = orig._list;
		_element = orig._element;
	}
	return *this;
}

#endif /* INDEXED_LIST_HPP */

