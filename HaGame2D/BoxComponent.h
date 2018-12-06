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
	~BoxComponent();
};

