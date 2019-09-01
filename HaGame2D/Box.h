#pragma once
#include "SDL.h"
#include "Vector.h"

struct Box {
	float x;
	float y;
	float width;
	float height;

	bool boundedBy(Box otherBox) {
		return (x >= otherBox.x && x + width <= otherBox.x + otherBox.width && y >= otherBox.y && y + height <= otherBox.y + otherBox.height);
	}

	SDL_Rect * getSDLRectPointer() {
		SDL_Rect * rect;
		rect->x = x;
		rect->y = y;
		rect->w = width;
		rect->h = height;
		return rect;
	}

	SDL_Rect getSDLRect() {
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = width;
		rect.h = height;
		return rect;
	}

	static Box empty() {
		Box newBox = Box{ 0, 0, 0, 0 };
		return newBox;
	}
};