/*
 * ILpSolver.cpp
 *
 *  Created on: 8 ao�t 2016
 *      Author: manuruiz
 */

#include "ILpSolver.h"

#define __SIGNATURE__ __PRETTY_FUNCTION__

ILpSolver::ILpSolver() {
	minimize();
	// TODO Auto-generated constructor stub

}

ILpSolver::~ILpSolver() {
	// TODO Auto-generated destructor stub
}

bool & ILpSolver::is_minimize() {
	return _is_minimize;
}
bool ILpSolver::is_minimize() const {
	return _is_minimize;
}

void ILpSolver::minimize() {
	_is_minimize = true;
}
void ILpSolver::maximize() {
	_is_minimize = false;
}

std::list<std::ostream *> & ILpSolver::stream() {
	return _stream;
}
void ILpSolver::add(std::ostream & stream) {
	_stream.push_back(&stream);
}

void ILpSolver::add(RowBuffer &) {
	throw std::invalid_argument(__SIGNATURE__);
}
void ILpSolver::add(ColumnBuffer &) {
	throw std::invalid_argument(__SIGNATURE__);
}

void ILpSolver::write(std::string const & fileName) const {
	throw std::invalid_argument(__SIGNATURE__);
}

void ILpSolver::initLp(std::string const & name) {
	throw std::invalid_argument(__SIGNATURE__);
}
void ILpSolver::freeLp() {
	throw std::invalid_argument(__SIGNATURE__);
}

void ILpSolver::chgObj(IntVector const & indexe, DoubleVector const & values) {
	throw std::invalid_argument(__SIGNATURE__);
}
void ILpSolver::delMipStarts() {
	throw std::invalid_argument(__SIGNATURE__);
}

bool ILpSolver::isOptimal() const {
	throw std::invalid_argument(__SIGNATURE__);
	return false;
}
double ILpSolver::objValue() const {
	throw std::invalid_argument(__SIGNATURE__);
	return -1;
}
void ILpSolver::objValue(int i, Double & obj) const {
	throw std::invalid_argument(__SIGNATURE__);

}
void ILpSolver::solution(int i, DoubleVector & result) const {
	throw std::invalid_argument(__SIGNATURE__);

}

int ILpSolver::ncols() const {
	throw std::invalid_argument(__SIGNATURE__);
	return -1;

}
int ILpSolver::nrows() const {
	throw std::invalid_argument(__SIGNATURE__);
	return -1;
}
char ILpSolver::binary() const {
	throw std::invalid_argument(__SIGNATURE__);
	return '\0';
}
double ILpSolver::infinity() const {
	throw std::invalid_argument(__SIGNATURE__);
	return -1;
}
char ILpSolver::continuous() const {
	throw std::invalid_argument(__SIGNATURE__);
	return '\0';
}
char ILpSolver::leq() const {
	throw std::invalid_argument(__SIGNATURE__);
	return '\0';
}
char ILpSolver::eq() const {
	throw std::invalid_argument(__SIGNATURE__);
	return '\0';
}
char ILpSolver::geq() const {
	throw std::invalid_argument(__SIGNATURE__);
	return '\0';
}

void ILpSolver::run() {
	throw std::invalid_argument(__SIGNATURE__);
}
void ILpSolver::setLog() {
	throw std::invalid_argument(__SIGNATURE__);
}
void ILpSolver::setNoLog() {
	throw std::invalid_argument(__SIGNATURE__);
}
void ILpSolver::setNbThreads(int) {
	throw std::invalid_argument(__SIGNATURE__);
}

