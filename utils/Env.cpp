/*
 * Env.cpp
 *
 *  Created on: 10 août 2012
 *      Author: manuel
 */

#include "Env.hpp"

Env Env::_Singleton = Env();

void Env::set(std::ostream & stream) {
	_Stream = &stream;
}
void Env::setNull() {
	_Stream = 0;
}

std::ostream * Env::get() {
	return _Stream;
}

Env & Env::Get() {
	return _Singleton;
}

Env::Env() :
		_Stream(&std::cout) {
}
