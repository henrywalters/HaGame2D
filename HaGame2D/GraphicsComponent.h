#pragma once
#include "Component.h"
#include "Display.h"
#include "Color.h"

class GraphicsComponent : public Component
{

public:

	float width, height;

	GraphicsComponent();
	~GraphicsComponent();
};

