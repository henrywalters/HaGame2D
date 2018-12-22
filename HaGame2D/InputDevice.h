#pragma once
#include "Vector.h"
#include <SDL.h>

struct Axis {
	Vector axis = Vector::Zero();

	bool isValid() {
		return (axis.x <= 1 && axis.x >= -1 && axis.y <= 1 && axis.y >= -1);
	}

	void set(Vector _axis) {
		axis = _axis;
		axis.normalize();
	}
};

class InputDevice
{
public:

	Axis axis1;
	Axis axis2;

	bool button1, button2, button3, button4;

	bool fire1, fire2, subfire1, subfire2;

	InputDevice();
	~InputDevice();

	virtual void initialize() {};
	virtual void handleEvent(SDL_Event &eventHandle) {};
};

