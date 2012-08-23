/*
 * Timer.hpp
 *
 *  Created on: 29 mars 2012
 *      Author: manuel
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "src/common.h"

#ifdef _WIN32 
#include <boost/timer.hpp>
typedef boost::timer TimePoint;
#else
#include <chrono>
typedef std::chrono::time_point<std::chrono::system_clock> TimePoint;
#endif

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
#ifdef _WIN32
	_start.restart();
#else
	_start = std::chrono::system_clock::now();
#endif
}

inline Double Timer::elapsed() const {
#ifdef _WIN32
	return _start.elapsed();
#else
	return std::chrono::duration < Double
			> (std::chrono::system_clock::now() - _start).count();
#endif
}

inline Timer::~Timer() {
}

#endif /* TIMER_HPP_ */
