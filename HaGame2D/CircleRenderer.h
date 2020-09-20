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

	std::vector<Vector> pixels;

public:

	RGB color = Color::blue();
	bool fill = false;

	CircleRenderer(double radius, bool positionCenter = true) : CircleComponent(radius, positionCenter) {}

	void onCreate() {
		pixels = {};
		int r = getRadius();
		int x = -r;
		int y = 0;
		int err = 2 - 2 * r;

		double xPos = getCircle().center.x;
		double yPos = getCircle().center.y;

		do {
			pixels.push_back(Vector(xPos - x, yPos + y));
			pixels.push_back(Vector(xPos - y, yPos - x));
			pixels.push_back(Vector(xPos + x, yPos - y));
			pixels.push_back(Vector(xPos + y, yPos + x));

			r = err;

			if (r <= y) err += ++y * 2 + 1;
			if (r > x || err > y) err += ++x * 2 + 1;

		} while (x < 0);
	}

	void update() {
		for (auto pixel : pixels) {
			display->drawPixel(transform->position + pixel, color);
		}
	}
};

