#pragma once
#include "Component.h"

class BoxComponent : public Component
{
public:

	float width;
	float height;

	float relativeWidth;
	float relativeHeight;

	BoxComponent();
	BoxComponent(float w, float h) {
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

