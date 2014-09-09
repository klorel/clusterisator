/*
 * Decision.cpp
 *
 *  Created on: 11 août 2013
 *      Author: manuel
 */

#include "Decision.hpp"

Decision::Decision() :
		_noeud1(0), _noeud2(0), _cannot(0), _empty(true) {

}
Decision::Decision(size_t noeud1, size_t noeud2, bool cannot) :
		_noeud1(noeud1), _noeud2(noeud2), _cannot(cannot), _empty(false) {

}
Decision::Decision(std::pair<size_t, size_t> noeud1noeud2, bool cannot) :
		_noeud1(noeud1noeud2.first), _noeud2(noeud1noeud2.second), _cannot(
				cannot), _empty(false) {

}
Decision::~Decision() {

}
Decision::Decision(Decision const & rhs) :
		_noeud1(rhs._noeud1), _noeud2(rhs._noeud2), _cannot(rhs._cannot), _empty(
				rhs._empty) {

}
Decision & Decision::operator=(Decision const & rhs) {
	if (&rhs != this) {
		_noeud1 = rhs._noeud1;
		_noeud2 = rhs._noeud2;
		_cannot = rhs._cannot;
		_empty = rhs._empty;
	}
	return *this;

}
bool Decision::cannot() const {
	return _cannot;
}
bool &Decision::cannot() {
	return _cannot;
}
size_t Decision::noeud1() const {
	return _noeud1;
}
size_t &Decision::noeud1() {
	return _noeud1;
}
size_t Decision::noeud2() const {
	return _noeud2;
}
size_t &Decision::noeud2() {
	return _noeud2;
}

bool Decision::operator<(Decision const & rhs) const {
	return std::less<std::pair<size_t, size_t>>()(
			std::make_pair(_noeud1, _noeud2),
			std::make_pair(rhs.noeud1(), rhs.noeud2()));
}

void Decision::print(std::ostream & stream) const {
	if (!_empty) {
		stream << (cannot() ? "C" : "M");
		stream << "_R" << noeud1();
		stream << "_B" << noeud2();
	}
}
std::string Decision::name() const {
	std::stringstream buffer;
	print(buffer);
	return buffer.str();
}
void Decision::Print(DecisionList const & list, std::ostream & stream) {
	for (auto const & d : list) {
		d.print(stream);
		stream << std::endl;
	}
}
void Decision::Print(DecisionSet const &list, std::ostream & stream) {
	for (auto const & d : list) {
		d.print(stream);
		stream << std::endl;
	}
}
size_t Decision::violation(bool isNoeud1, bool isNoeud2) const {
	size_t result(0);
	if (cannot()) {
		if (isNoeud1 && isNoeud2)
			++result;
	} else {
		if (isNoeud1 == !isNoeud2)
			++result;
	}
	return result;
}
