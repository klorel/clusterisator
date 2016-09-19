/*
 * Stabilisators.cpp
 *
 *  Created on: 17 sept. 2016
 *      Author: manuel
 */

#include "Stabilisator.h"

Stabilisator::Stabilisator() :
		_pi_bar(), _penalty(0), _width(0) {

}
Stabilisator::Stabilisator(DoubleVector const & pi, Double penalty, Double width) {
	set(pi, penalty, width);

}

void Stabilisator::set(DoubleVector const & pi, Double penalty, Double width) {
	_pi_bar = pi;
	_penalty = penalty;
	_width = width;
}

DoubleVector &Stabilisator::pi_bar() {
	return _pi_bar;
}
DoubleVector const &Stabilisator::pi_bar() const {
	return _pi_bar;
}

Double Stabilisator::delta1(int i) const {
	return _pi_bar[i] - _width;
}
Double Stabilisator::delta2(int i) const {
	return _pi_bar[i] + _width;
}
Double Stabilisator::gamma1(int i) const {
	return delta1(i) - _width * 2;
}
Double Stabilisator::gamma2(int i) const {
	return delta2(i) + _width * 2;
}

Double Stabilisator::dseta_m() const {
	return std::pow(_penalty, 5);
}
Double Stabilisator::epsilon_m() const {
	return _penalty;
}
Double Stabilisator::epsilon_p() const {
	return _penalty;
}
Double Stabilisator::dseta_p() const {
	return std::pow(_penalty, 5);
}

void Stabilisator::build(CplexSolver & solver, ColumnBuffer & buffer) {
	buffer.clear();
	char const c(solver.continuous());
	int n_rows((int) _pi_bar.size());
	for (int row(0); row < n_rows; ++row) {
		buffer.add(0, c, 0, dseta_m() + epsilon_m(), GetStr("z_m_", row));
		buffer.add(row, +1);
		buffer.add(0, c, 0, epsilon_m(), GetStr("y_m_", row));
		buffer.add(row, +1);
		buffer.add(0, c, 0, epsilon_p(), GetStr("y_p_", row));
		buffer.add(row, -1);
		buffer.add(0, c, 0, dseta_p() + epsilon_p(), GetStr("z_p_", row));
		buffer.add(row, -1);
	}
}
void Stabilisator::update_pi(CplexSolver & solver, DoubleVector const & pi) {
	DoubleVector rc(_cost);
	solver.rc(rc);
	int n_rows((int) _pi_bar.size());
//	size_t i(0);
//	for (int row(0); row < n_rows; ++row) {
//		std::cout << GetStr("z_m_", row) << " : " << rc[i] << std::endl;
//		++i;
//		std::cout << GetStr("y_m_", row) << " : " << rc[i] << std::endl;
//		++i;
//		std::cout << GetStr("y_p_", row) << " : " << rc[i] << std::endl;
//		++i;
//		std::cout << GetStr("z_p_", row) << " : " << rc[i] << std::endl;
//		++i;
//	}

	if (_pi_bar.empty()) {
		_pi_bar = pi;
		for (auto & v : _pi_bar) {
			v *= -1;
		}
	} else {
		for (size_t i(0); i < _pi_bar.size(); ++i) {
			_pi_bar[i] += 0.9 * (-pi[i] - _pi_bar[i]);
		}
	}
//	std::cout << "avg = " << avg / _pi_bar.size() << std::endl;
	_cost.assign(4 * _pi_bar.size(), 0);
	_indexes.assign(4 * _pi_bar.size(), 0);

	int index(-1);
	for (int row(0); row < n_rows; ++row) {
		_cost[++index] = gamma1(row);
		_indexes[index] = index;
		_cost[++index] = delta1(row);
		_indexes[index] = index;
		_cost[++index] = -delta2(row);
		_indexes[index] = index;
		_cost[++index] = -gamma2(row);
		_indexes[index] = index;
	}
	solver.chgObj(_indexes, _cost);
}
Double Stabilisator::get_penalty(CplexSolver & solver) {
	solver.primal(_x);
	Double result(0);
	for (auto const id : _indexes) {
		result += _cost[id] * _x[id];
	}
	return result;
}
