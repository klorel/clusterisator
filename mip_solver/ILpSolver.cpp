/*
 * ILpSolver.cpp
 *
 *  Created on: 8 août 2016
 *      Author: manuruiz
 */

#include "ILpSolver.h"

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
bool ILpSolver::is_minimize()const {
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
	throw std::invalid_argument("UNIMPLEMENTED");
}
void ILpSolver::add(ColumnBuffer &) {
	throw std::invalid_argument("UNIMPLEMENTED");
}

void ILpSolver::write(std::string const & fileName) const {
	throw std::invalid_argument("UNIMPLEMENTED");
}

void ILpSolver::initLp(std::string const & name) {
	throw std::invalid_argument("UNIMPLEMENTED");
}
void ILpSolver::freeLp() {
	throw std::invalid_argument("UNIMPLEMENTED");
}

void ILpSolver::chgObj(IntVector const & indexe, DoubleVector const & values) {
	throw std::invalid_argument("UNIMPLEMENTED");
}
void  ILpSolver::delMipStarts() {
	throw std::invalid_argument("UNIMPLEMENTED");
}

bool ILpSolver::isOptimal()const {
	throw std::invalid_argument("UNIMPLEMENTED");
	return false;
}
double ILpSolver::objValue()const {
	throw std::invalid_argument("UNIMPLEMENTED");
	return -1;
}

char ILpSolver::binary()const {
	throw std::invalid_argument("UNIMPLEMENTED"); return '\0';
}
char ILpSolver::continuous()const {
	throw std::invalid_argument("UNIMPLEMENTED"); return '\0';
}
char ILpSolver::leq()const {
	throw std::invalid_argument("UNIMPLEMENTED"); return '\0';
}
char ILpSolver::eq()const {
	throw std::invalid_argument("UNIMPLEMENTED"); return '\0';
}
char ILpSolver::geq()const {
	throw std::invalid_argument("UNIMPLEMENTED"); return '\0';
}

void ILpSolver::run() {
	throw std::invalid_argument("UNIMPLEMENTED");
}
