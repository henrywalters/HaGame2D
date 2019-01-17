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
};

