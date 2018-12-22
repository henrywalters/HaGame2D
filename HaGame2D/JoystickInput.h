#pragma once
#include "InputDevice.h"
#include <SDL.h>

const int JOYSTICK_DEADZONE = 4000;
const float JOYSTICK_RANGE = 32768;

class JoystickInput :
	public InputDevice
{
	SDL_Joystick *joystick;

	int joystickIndex;

public:

	JoystickInput(int jsIndex);
	~JoystickInput();

	void handleEvent(SDL_Event &e) {
		if (e.type == SDL_JOYAXISMOTION && e.jaxis.which == joystickIndex && abs(e.jaxis.value) >= JOYSTICK_DEADZONE) {
			if (e.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTX) {
				axis1.axis.x = e.jaxis.value / JOYSTICK_RANGE;
			}

			if (e.jaxis.axis == SDL_CONTROLLER_AXIS_LEFTY) {
				axis1.axis.y = e.jaxis.value / JOYSTICK_RANGE;
			}

			if (e.jaxis.axis == SDL_CONTROLLER_AXIS_RIGHTX) {
				axis2.axis.x = e.jaxis.value / JOYSTICK_RANGE;
			}

			if (e.jaxis.axis == SDL_CONTROLLER_AXIS_RIGHTY) {
				axis2.axis.y = e.jaxis.value / JOYSTICK_RANGE;
			}

			//std::cout << "AXIS 1 ";
			//axis1.axis.display();

			std::cout << "AXIS 2 ";
			axis2.axis.display();
		}
	}
};

