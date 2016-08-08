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


bool & ILpSolver::is_minimize(){
	return _is_minimize;
}
bool ILpSolver::is_minimize()const{
	return _is_minimize;
}

void ILpSolver::minimize(){
	_is_minimize = true;
}
void ILpSolver::maximize(){
	_is_minimize = false;
}

std::list<std::ostream *> & ILpSolver::stream() {
	return _stream;
}
void ILpSolver::add(std::ostream & stream) {
	_stream.push_back(&stream);
}
