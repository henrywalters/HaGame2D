#include "stdafx.h"
#include "Circle.h"


Circle::Circle(float _radius)
{
	radius = _radius;
}


Circle::~Circle()
{
}

void Circle::draw(Display *display) {
	SDL_SetRenderDrawColor(display->renderer, 0x00, 0x42, 0xAD, 0xFF);
}
