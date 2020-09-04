#pragma once
#include "SDL.h"
#include "Vector.h"
#include "Matrix.h"
#include "WorldMapper.h"
#include <string>

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

	void globalizeMouse(float xOffset, float yOffset, Matrix screen, Matrix viewport) {
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

