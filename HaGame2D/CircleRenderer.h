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

	RGB color = Color::blue();
	bool fill = false;

	CircleRenderer(float radius, bool positionCenter = true) : CircleComponent(radius, positionCenter) {}

	void update() {
		int r = getRadius();
		int x = -r;
		int y = 0;
		int err = 2 - 2 * r;

		float xPos = getCircle().center.x;
		float yPos = getCircle().center.y;

		do {
			display->drawPixel(xPos - x, yPos + y, color);
			display->drawPixel(xPos - y, yPos - x, color);
			display->drawPixel(xPos + x, yPos - y, color);
			display->drawPixel(xPos + y, yPos + x, color);

			r = err;

			if (r <= y) err += ++y * 2 + 1;
			if (r > x || err > y) err += ++x * 2 + 1;

		} while (x < 0);
	}
};

