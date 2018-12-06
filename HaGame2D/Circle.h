#pragma once
#include "GameObject.h"
class Circle :
	public GameObject
{
public:

	float radius;

	Circle(float radius);
	~Circle();

	void draw(Display *display);
};

