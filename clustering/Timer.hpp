/*
 * Timer.hpp
 *
 *  Created on: 29 mars 2012
 *      Author: manuel
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "common.h"

#include <chrono>
typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;

class Timer {
public:
	Timer();
	virtual ~Timer();

	Double elapsed() const;
	void restart();
private:
	TimePoint _start;
};

inline Timer::Timer() {
	restart();

}

inline void Timer::restart() {
	_start = std::chrono::system_clock::now();

}

inline Double Timer::elapsed() const {
	return std::chrono::duration < Double
			> (std::chrono::system_clock::now() - _start).count();
}

inline Timer::~Timer() {
}

#endif /* TIMER_HPP_ */
