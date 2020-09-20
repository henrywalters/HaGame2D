#pragma once
#include <thread>
#include <chrono>

enum ClockPrecision {
	Second,
	Millisecond,
	Microsecond
};

class Clock
{
	long internalTime;

	static double castToInt(long timeInMicroSeconds, ClockPrecision precision);

public:

	Clock();

	static long now(ClockPrecision precision = ClockPrecision::Microsecond);

	void start();
	double stop(ClockPrecision precision = ClockPrecision::Microsecond);
	void wait(long timeInMicroSeconds);
};

