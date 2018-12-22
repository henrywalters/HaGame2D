#pragma once
#include "InputDevice.h"
#include "JoystickInput.h"
#include "KeyboardMouseInput.h"
#include <SDL.h>

const int MAX_INPUT_DEVICES = 5;
const int MAX_PLAYERS = 4;

class InputManager
{

	//Raw input order
	InputDevice *inputDevices[MAX_INPUT_DEVICES];
	
	//Player 1,2,3 ...
	InputDevice *players[MAX_PLAYERS];

	int deviceCount = 0;
	int playerCount = 0;

public:
	InputManager();
	~InputManager();

	int availableJoysticks();
	
	void poll() {
		
		SDL_Event event;

		while (SDL_PollEvent(&event) != 0) {
			for (int i = 0; i < deviceCount; i++) {
				inputDevices[i]->handleEvent(event);
			}
		}
	}
};

