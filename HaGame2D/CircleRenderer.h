#pragma once
#include "GameObject.h"
#include "Line.h"
#include "CircleComponent.h"
#include <vector>

class CircleRenderer :
	public CircleComponent
{
private:

	std::vector<Line> lines;

public:

	CircleRenderer(float radius) : CircleComponent(radius) {}

	void update() {
		int r = getCircle().getRadius();
		int x = -r;
		int y = 0;
		int err = 2 - 2 * r;

		float xPos = transform->position.x;
		float yPos = transform->position.y;

		do {
			display->drawPixel(xPos - x, yPos + y, Color::blue());
			display->drawPixel(xPos - y, yPos - x, Color::blue());
			display->drawPixel(xPos + x, yPos - y, Color::blue());
			display->drawPixel(xPos + y, yPos + x, Color::blue());

			r = err;

			if (r <= y) err += ++y * 2 + 1;
			if (r > x || err > y) err += ++x * 2 + 1;

		} while (x < 0);
	}
};

