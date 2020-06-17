#pragma once
#include "HaGame2D.h"

class IntervalComponent : public Component {
	long _lastTick;
	float _tickDelay;

public:

	IntervalComponent(float tickDelay) {
		_tickDelay = tickDelay;
		_lastTick = Clock::now();
	}

	virtual void updateEveryTick() {};
	virtual void updateEveryInterval(float dt) = 0;

	void update() {
		updateEveryTick();
		
		long now = Clock::now();
		float dt = now - _lastTick;

		if (dt >= _tickDelay) {
			_lastTick = now;
			updateEveryInterval(dt);
		}
	}
};