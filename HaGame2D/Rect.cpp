#include "stdafx.h"
#include "Rect.h"
#include <iostream>

Rect::Rect(float _width, float _height)
{
	width = _width;
	height = _height;
}


Rect::~Rect()
{
}

void Rect::draw(Display *display) {
	SDL_Rect fillRect = { position.x, position.y, width, height };
	SDL_SetRenderDrawColor(display->renderer, 0x00, 0x42, 0xAD, 0xFF);
	SDL_RenderFillRect(display->renderer, &fillRect);
}