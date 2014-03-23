/*
 * Decision.h
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#ifndef DECISION_HPP
#define DECISION_HPP

#include "gencol.h"
class Decision {
public:
	Decision();
	Decision(Decision const &);
	Decision(size_t, size_t, bool);
	Decision(std::pair<size_t, size_t> rb, bool = false);
	Decision & operator=(Decision const &);
	virtual ~Decision();
	bool cannot() const;
	bool &cannot();
	size_t r() const;
	size_t &r();
	size_t b() const;
	size_t &b();

	bool operator<(Decision const &) const;
	void print(std::ostream & = std::cout) const;
	std::string name() const;

	static void Print(DecisionList const &, std::ostream & = std::cout);
	static void Print(DecisionSet const &, std::ostream & = std::cout);

	size_t violation(bool, bool) const;
private:
	size_t _r;
	size_t _b;
	bool _cannot;
	bool _empty;
};

inline Decision::Decision() :
		_r(0), _b(0), _cannot(0), _empty(true) {

}
inline Decision::Decision(size_t r, size_t b, bool cannot) :
		_r(r), _b(b), _cannot(cannot), _empty(false) {

}
inline Decision::Decision(std::pair<size_t, size_t> rb, bool cannot) :
		_r(rb.first), _b(rb.second), _cannot(cannot), _empty(false) {

}
inline Decision::~Decision() {

}
inline Decision::Decision(Decision const & rhs) :
		_r(rhs._r), _b(rhs._b), _cannot(rhs._cannot), _empty(rhs._empty) {

}
inline Decision & Decision::operator=(Decision const & rhs) {
	if (&rhs != this) {
		_r = rhs._r;
		_b = rhs._b;
		_cannot = rhs._cannot;
		_empty = rhs._empty;
	}
	return *this;

}
inline bool Decision::cannot() const {
	return _cannot;
}
inline bool &Decision::cannot() {
	return _cannot;
}
inline size_t Decision::r() const {
	return _r;
}
inline size_t &Decision::r() {
	return _r;
}
inline size_t Decision::b() const {
	return _b;
}
inline size_t &Decision::b() {
	return _b;
}

inline bool Decision::operator<(Decision const & rhs) const {
	return std::less<std::pair<size_t, size_t>>()(std::make_pair(_r, _b),
			std::make_pair(rhs.r(), rhs.b()));
}

inline void Decision::print(std::ostream & stream) const {
	if (!_empty) {
		stream << (cannot() ? "C" : "M");
		stream << "_R" << r();
		stream << "_B" << b();
	}
}
inline std::string Decision::name() const {
	std::stringstream buffer;
	print(buffer);
	return buffer.str();
}
inline void Decision::Print(DecisionList const & list, std::ostream & stream) {
	for (auto const & d : list) {
		d.print(stream);
		stream << std::endl;
	}
}
inline void Decision::Print(DecisionSet const &list, std::ostream & stream) {
	for (auto const & d : list) {
		d.print(stream);
		stream << std::endl;
	}
}
inline size_t Decision::violation(bool isR, bool isB) const {
	size_t result(0);
	if (cannot()) {
		if (isR && isB)
			++result;
	} else {
		if (isR == !isB)
			++result;
	}
	return result;
}
#endif /* DECISION_H_ */
