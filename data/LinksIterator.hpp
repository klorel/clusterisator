/*
 * LinkIterator.hpp
 *
 *  Created on: 10 août 2012
 *      Author: manuel
 */

#ifndef LINKITERATOR_HPP_
#define LINKITERATOR_HPP_

#include "../utils/common.h"

class LinksIterator {
public:
	LinksIterator(Links::const_iterator begin, Links::const_iterator end) {
		_current = begin;
		_previous = _current;
		_end = end;
	}
	virtual ~LinksIterator() {

	}

	bool goNext() {
		if (_current != _end) {
			_previous = _current;
			++_current;
			return true;
		}
		return false;
	}
	size_t index() const {
		return _previous->first;
	}
	Double  value() const {
		return _previous->second;
	}
private:
	Links::const_iterator _previous;
	Links::const_iterator _current;
	Links::const_iterator _end;
};

#endif /* LINKITERATOR_HPP_ */
