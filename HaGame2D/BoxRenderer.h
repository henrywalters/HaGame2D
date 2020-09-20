#pragma once
#include "Component.h"
#include "BoxComponent.h"
#include "GameObject.h"
#include "Color.h"
#include "Vector.h"

class BoxRenderer : public BoxComponent
{
public:

	RGB color = Color::white();

	bool fill = false;

	BoxRenderer(double _width, double _height);
	BoxRenderer(double _width, double _height, bool _fill) {
		width = _width;
		height = _height;
		fill = _fill;
	}
	BoxRenderer(double _width, double _height, bool _fill, RGB _color) {
		width = _width;
		height = _height;
		fill = _fill;
		color = _color;
	}
	~BoxRenderer();

	void update() override;
};

