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

	BoxRenderer(float _width, float _height);
	BoxRenderer(float _width, float _height, bool _fill) {
		width = _width;
		height = _height;
		fill = _fill;
	}
	BoxRenderer(float _width, float _height, bool _fill, RGB _color) {
		width = _width;
		height = _height;
		fill = _fill;
		color = _color;
	}
	~BoxRenderer();

	void update() override;
};

