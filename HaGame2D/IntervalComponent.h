#pragma once
#include "HaGame2D.h"

class IntervalComponent : public Component {
	long _lastTick;
	double _tickDelay;

public:

	IntervalComponent(double tickDelay) {
		_tickDelay = tickDelay;
		_lastTick = Clock::now();
	}

	virtual void updateEveryTick() {};
	virtual void updateEveryInterval(double dt) = 0;

	void update() {
		updateEveryTick();
		
		long now = Clock::now();
		double dt = now - _lastTick;

		if (dt >= _tickDelay) {
			_lastTick = now;
			updateEveryInterval(dt);
		}
	}
};