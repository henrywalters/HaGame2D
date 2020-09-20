#pragma once
#include "HaGame2D.h"

template <size_t size>
class PolygonRenderer : public Component {
public:
	RGB color = Color::blue();
	bool fill = false;
	Polygon<size> polygon;

	template <size_t size>
	PolygonRenderer(Polygon<size> _polygon) {
		polygon = _polygon;
	}

	void update() {
		if (polygon.size == 1) {
			gameScene->display->drawPixel(polygon.vertices[0], color);
		}
		else if (polygon.size > 1) {
			for (int i = 1; i <= polygon.size; i++) {
				display->drawLine(transform->relativePosition + polygon.vertices[i - 1], transform->relativePosition + polygon.vertices[i % size], color);
			}
		}
	}
};