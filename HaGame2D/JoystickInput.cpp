#include "stdafx.h"
#include "JoystickInput.h"


JoystickInput::JoystickInput(int jsIndex)
{
	joystickIndex = jsIndex;
	joystick = SDL_JoystickOpen(joystickIndex);

	if (joystick == NULL) {
		printf("WARNING - failed to connect to joypad with error: %s\n", SDL_GetError());
	}
}


JoystickInput::~JoystickInput()
{
}
