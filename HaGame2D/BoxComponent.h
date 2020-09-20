#pragma once
#include "Component.h"

class BoxComponent : public Component
{
public:

	double width;
	double height;

	double relativeWidth;
	double relativeHeight;

	BoxComponent();
	BoxComponent(double w, double h) {
		width = w;
		height = h;
	}
	~BoxComponent();

	void setSize(Vector size) {
		width = size.x;
		height = size.y;
	}

	Vector getRelativeScale() {
		return Vector(relativeWidth / width, relativeHeight / height);
	}
};

