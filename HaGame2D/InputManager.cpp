#include "stdafx.h"
#include "InputManager.h"


InputManager::InputManager()
{
	SDL_Init(SDL_INIT_JOYSTICK);

	inputDevices[0] = new KeyboardMouseInput();
	deviceCount += 1;

	for (int i = 0; i < availableJoysticks(); i++) {
		inputDevices[i + 1] = new JoystickInput(i);
		deviceCount += 1;
	}
}


InputManager::~InputManager()
{
}

int InputManager::availableJoysticks()
{
	int joysticks = SDL_NumJoysticks();
	return joysticks;
}
