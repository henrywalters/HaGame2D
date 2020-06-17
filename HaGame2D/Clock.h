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

	static float castToInt(long timeInMicroSeconds, ClockPrecision precision);

public:

	Clock();

	static long now(ClockPrecision precision = ClockPrecision::Microsecond);

	void start();
	float stop(ClockPrecision precision = ClockPrecision::Microsecond);
	void wait(long timeInMicroSeconds);
};

