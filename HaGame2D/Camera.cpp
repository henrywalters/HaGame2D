#include "stdafx.h"
#include "Camera.h"
#include <iostream>

Camera::Camera()
{
	focus = Vector(screenWidth / 2, screenHeight / 2);
	zoomFactor = 1;
}


Camera::~Camera()
{
}


void Camera::lerp(Vector newOrigin, int frames) {
	lerpStart = Vector(viewport.get(0), viewport.get(1));
	lerpEnd = newOrigin;

	float distance = (lerpEnd - lerpStart).magnitude();

	
	
	if (distance > 0) {
		lerping = true;
		lerpFrames = frames;
		currentLerpFrame = 0;

		//distance * (1 / lerpFrames / distance) 
		lerpPerFrame = (lerpEnd - lerpStart).normalized() * (distance / lerpFrames);
	}
}

void Camera::dolly(Vector vel) {
	dollying = true;
	dollyVelocity = vel;
}

void Camera::stopDollying() {
	dollying = false;
}

void Camera::update() {
	if (lerping) {
		if (currentLerpFrame < lerpFrames) {
			move(lerpPerFrame);
			currentLerpFrame += 1;
		}
		else {
			lerping = false;
		}
	}

	if (dollying) {
		move(dollyVelocity);
	}

	//SDL_ShowCursor(SDL_DISABLE);

	//focus.x = input->mouseX;
	//focus.y = input->mouseY;

	if (drawCursor) {
		display->drawLine(focus.x - 15, focus.y, focus.x + 15, focus.y, Color::blue(), 18);
		display->drawLine(focus.x, focus.y - 15, focus.x, focus.y + 15, Color::blue(), 18);
	}
}