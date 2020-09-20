#include "stdafx.h"
#include "Clock.h"
#include <iostream>
double Clock::castToInt(long ms, ClockPrecision precision)
{
	switch (precision) {
	case ClockPrecision::Microsecond:
		return ms;
	case ClockPrecision::Millisecond:
		return ms / 1000;
	case ClockPrecision::Second:
		return ms / 1000000;
	}
}

Clock::Clock()
{
}

long Clock::now(ClockPrecision precision)
{
	std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::microseconds>(time.time_since_epoch()).count();
}

void Clock::start()
{
	internalTime = now();
}

double Clock::stop(ClockPrecision precision)
{
	long stop = now();
	return castToInt(stop - internalTime, precision);
}

void Clock::wait(long timeInMicroSeconds)
{
	//std::this_thread::sleep_for(std::chrono::microseconds(timeInMicroSeconds));
	double elapsed = 0;
	long start = Clock::now();
	while (elapsed < timeInMicroSeconds) {
		elapsed = Clock::now() - start;
	}
}
