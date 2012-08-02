/*
 * Timer.hpp
 *
 *  Created on: 29 mars 2012
 *      Author: manuel
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

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

	double elapsed() const;
	void restart();
private:
	TimePoint _start;
};

#endif /* TIMER_HPP_ */
