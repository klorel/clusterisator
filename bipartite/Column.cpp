#include "Column.hpp"

Column::Column(BipartiteGraph const & input) :
		_input(&input), _cost(0), _reducedCost(0), _r(), _b(), _id(0) {
}
Column::Column(BipartiteGraph const * input) :
		_input(input), _cost(0), _reducedCost(0), _r(), _b(), _id(0) {
}
Column::~Column() {
}

Double Column::computeCost() const {
	Double result(0);
	for (auto const & r : _r) {
		for (auto const & b : _b) {
			//std::cout << std::setw(6)<<r;
			//std::cout << std::setw(6)<<_input->kR(r);
			//std::cout << std::setw(6)<<b;
			//std::cout << std::setw(6)<<_input->kB(b);
			//std::cout << std::setw(25)<< _input->w(r,b);
			//std::cout << std::endl;
			result += _input->w(r, b);
		}
	}
	return result;
}
Double Column::computeReducedCost(DoubleVector const & dual) const {
	Double result(0);
	for (auto const & r : _r) {
		for (auto const & b : _b) {
			result += _input->w(r, b);
		}
	}
	for (auto const & r : _r) {
		result += dual[r];
	}
	for (auto const & b : _b) {
		result += dual[_input->nR() + b];
	}
	return result;
}
size_t Column::size() const {
	return _r.size() + _b.size();
}

Double & Column::cost() {
	return _cost;
}
Double const & Column::cost() const {
	return _cost;
}
Double & Column::reducedCost() {
	return _reducedCost;
}
Double const & Column::reducedCost() const {
	return _reducedCost;
}
void Column::delElement(size_t i) {
	if (i > _input->nR())
		_b.erase(i - _input->nR() - 1);
	else
		_r.erase(i - 1);

}
void Column::addElement(size_t i) {
	if (i > _input->nR())
		_b.insert(i - _input->nR() - 1);
	else
		_r.insert(i - 1);
}

size_t & Column::id() const {
	return _id;
}

void Column::check(DoubleVector const & dual) const {
	if (std::fabs(computeCost() - _cost) > 1e-6) {
		std::cout << "wrong cost " << std::endl;
		std::cout << _cost << std::endl;
		std::cout << computeCost() << std::endl;
		assert(false && "WRONG COST");
	}
	if (std::fabs(computeReducedCost(dual) - _reducedCost) > 1e-6) {
		std::cout << "wrong reduced cost " << std::endl;
		std::cout << _reducedCost << std::endl;
		std::cout << computeReducedCost(dual) << std::endl;
//		for (auto const v : dual) {
//			std::cout << v << std::endl;
//		}
		assert(false && "WRONG REDUCED COST");
	}

}

void Column::print(std::ostream & stream) const {
	std::copy(_r.begin(), _r.end(),
			std::ostream_iterator<size_t>(stream << "R : ", " "));
	std::copy(_b.begin(), _b.end(),
			std::ostream_iterator<size_t>(stream << "\nB : ", " "));
	stream << std::endl;
}
std::set<size_t> & Column::r() {
	return _r;
}
std::set<size_t> const & Column::r() const {
	return _r;
}

std::set<size_t> & Column::b() {
	return _b;
}
std::set<size_t> const & Column::b() const {
	return _b;
}
bool Column::operator<(Column const & rhs) const {
	if (_r == rhs.r())
		return _b < rhs.b();
	else
		return _r < rhs.r();
}

size_t Column::violation(DecisionList const & decisions) const {
	size_t result(0);
	for (auto const & decision : decisions) {
		result += violation(decision);

	}
	return result;

}
size_t Column::violation(Decision const & decision) const {
	bool const isR(_r.find(decision.r()) != _r.end());
	bool const isB(_b.find(decision.b()) != _b.end());
//	MY_PRINT(isR);
//	MY_PRINT(isB);
	return decision.violation(isR, isB);
}
bool Column::constains(size_t r, size_t b) const {
	return constainsR(r) && constainsB(b);

}

bool Column::constainsR(size_t r) const {
	return _r.find(r) != _r.end();

}
bool Column::constainsB(size_t b) const {
	return _b.find(b) != _b.end();

}
