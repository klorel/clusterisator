/*
 * Timer.cpp
 *
 *  Created on: 29 mars 2012
 *      Author: manuel
 */

#include "Timer.hpp"

Timer::Timer() {
	restart();

}

void Timer::restart() {
#ifdef _WIN32 	
	_start.restart();
#else
	_start = std::chrono::system_clock::now();
#endif
}

double Timer::elapsed() const {
#ifdef _WIN32 	
	return _start.elapsed();
#else
	return std::chrono::duration<double>(std::chrono::system_clock::now() - _start).count();
#endif
}

Timer::~Timer() {
}

