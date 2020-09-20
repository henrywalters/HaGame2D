#pragma once
#include "GameObject.h"
#include "SDL.h"

class Rect : public GameObject
{
public:

	double width;
	double height;

	Vector velocity = Vector(0, 0);

	Rect(double width, double height);
	~Rect();

	void draw(Display * display) override;
};

