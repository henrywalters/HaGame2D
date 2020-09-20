#pragma once
#include "SDL.h"
#include "Vector.h"
#include "Matrix.h"
#include "WorldMapper.h"
#include <string>

// Really low deadzone by default. Let the client decide their own thresholds.
const int JOYSTICK_DEADZONE = 500;

struct InputDeviceState {
	Vector rAxis;
	Vector lAxis;
	Vector dPad;

	bool select, selectPressed;
	bool home, homePressed;
	bool start, startPressed;

	bool a, aPressed;
	bool b, bPressed;
	bool x, xPressed;
	bool y, yPressed;

	float rTriggerRaw, lTriggerRaw;
	bool lTrigger, lTriggerPressed;
	bool rTrigger, rTriggerPressed;

	bool lShoulder, lShoulderPressed;
	bool rShoulder, rShoulderPressed;
};

class InputDevice : public InputDeviceState {
public:
	virtual void pollDevice() {};
};

class Gamepad : public InputDevice {
private:
	SDL_GameController* joystick;
	SDL_Haptic* haptic;
	bool active = false;

	void checkButton(bool& btn, bool &btnPressed, SDL_GameControllerButton button) {
		checkButton(btn, btnPressed, SDL_GameControllerGetButton(joystick, button));
	}

	void checkButton(bool& btn, bool& btnPressed, bool state) {
		btnPressed = false;
		if (state) {
			if (!btn) btnPressed = true;
			btn = true;
		}
		else {
			btn = false;
		}
	}
public:

	Gamepad(int index) {
		joystick = SDL_GameControllerOpen(index);

		haptic = SDL_HapticOpen(index);
		SDL_HapticRumbleInit(haptic);
		if (joystick == NULL) {
			std::cout << "Warning: failed to open joystick " << index << " - Error: " << SDL_GetError() << std::endl;
		}
		else {
			active = true;
		}
	}

	void rumble(float strength, float dur) {
		if (active) {
			if (SDL_HapticRumblePlay(haptic, strength, dur) != 0) {
				std::cout << "Failed to rumble: " << SDL_GetError() << std::endl;
			}
		}
	}

	void pollDevice() {
		rAxis = Vector::Zero();
		lAxis = Vector::Zero();
		dPad = Vector::Zero();

		if (active) {
			lAxis.x = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTX);
			lAxis.y = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_LEFTY);
			rAxis.x = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_RIGHTX);
			rAxis.y = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_RIGHTY);

			lAxis.x = abs(lAxis.x) > JOYSTICK_DEADZONE ? lAxis.x / 32768.0 : 0;
			lAxis.y = abs(lAxis.y) > JOYSTICK_DEADZONE ? lAxis.y / 32768.0 : 0;
			rAxis.x = abs(rAxis.x) > JOYSTICK_DEADZONE ? rAxis.x / 32768.0 : 0;
			rAxis.y = abs(rAxis.y) > JOYSTICK_DEADZONE ? rAxis.y / 32768.0 : 0;

			checkButton(a, aPressed, SDL_CONTROLLER_BUTTON_A);
			checkButton(b, bPressed, SDL_CONTROLLER_BUTTON_B);
			checkButton(x, xPressed, SDL_CONTROLLER_BUTTON_X);
			checkButton(y, yPressed, SDL_CONTROLLER_BUTTON_Y);

			checkButton(select, selectPressed, SDL_CONTROLLER_BUTTON_BACK);
			checkButton(home, homePressed, SDL_CONTROLLER_BUTTON_GUIDE);
			checkButton(start, startPressed, SDL_CONTROLLER_BUTTON_START);

			checkButton(rShoulder, rShoulderPressed, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
			checkButton(lShoulder, lShoulderPressed, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);

			lTriggerRaw = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_TRIGGERLEFT);
			rTriggerRaw = SDL_GameControllerGetAxis(joystick, SDL_CONTROLLER_AXIS_TRIGGERRIGHT);

			checkButton(lTrigger, lTriggerPressed, lTriggerRaw >= JOYSTICK_DEADZONE);
			checkButton(rTrigger, rTriggerPressed, rTriggerRaw >= JOYSTICK_DEADZONE);
		}
	}
};

class Input
{
	Vector origin;
public:

	bool up = false; bool down = false; bool left = false; bool right = false;
	bool upPressed = false;
	bool downPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;

	bool space = false;

	bool q = false;

	bool shift = false;

	bool one = false;
	bool two = false;
	bool three = false;
	bool four = false;

	bool action = false;
	bool actionDown = false;

	bool quit;

	int mouseX, mouseY;
	int mouseXGlobal, mouseYGlobal;
	
	bool fire1 = false;
	bool fire2 = false;

	bool fire1Down = false;
	bool fire2Down = false;
	bool fire1Up = false;
	bool fire2Up = false;

	bool esc = false;

	bool enter = false;
	bool enterDown = false;

	int mouseScroll = 0;

	bool streamingText = false;
	std::string currentTextStream = "";
	std::string textStream = "";

	Input();
	~Input();

	void clear();

	void pollEvents();

	void setOrigin(Vector _origin) {
		origin = _origin;
	}

	void globalizeMouse(double xOffset, double yOffset, Matrix screen, Matrix viewport) {
		Matrix local = Matrix(Vector(mouseX, mouseY), Vector(0, 0));
		Matrix global = WorldMapper::screenToWorld(local, screen, viewport);
		mouseXGlobal = global.get(0) - xOffset;
		mouseYGlobal = global.get(1) - yOffset;
	}

	void readTextStream() {
		streamingText = true;
	}

	Vector mousePos() {
		return Vector(mouseX, mouseY);
	}

	Vector globalMousePos() {
		return Vector(mouseXGlobal, mouseYGlobal);
	}
};

