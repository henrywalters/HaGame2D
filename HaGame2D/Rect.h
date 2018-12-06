#pragma once
#include "GameObject.h"
#include "SDL.h"

class Rect : public GameObject
{
public:

	float width;
	float height;

	Vector velocity = Vector(0, 0);

	Rect(float width, float height);
	~Rect();

	void draw(Display * display) override;
};

