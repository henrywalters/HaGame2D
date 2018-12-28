#pragma once
#include "Vector.h"
#include "SDL.h"
#include "Display.h"
#include "Input.h"
#include "GameObject.h"
#include "BoxComponent.h"
#include "WorldMapper.h"

struct SceneBox {
	Vector origin;
	Vector size;
};

const float MIN_ZOOM = 0.01;
const float MAX_ZOOM = 20;

const float DEFAULT_ZOOM_SPEED = 0.01;

class Camera
{
	Matrix initialViewport;
	Matrix viewport;

	bool lerping = false;
	Vector lerpStart;
	Vector lerpEnd;
	int lerpFrames;
	int currentLerpFrame;
	Vector lerpPerFrame;

	Vector dollyVelocity;
	bool dollying = false;

	Vector focus;

	float screenWidth;
	float screenHeight;

	float zoomFactor;

	Display *display;
	Input * input;

	bool drawCursor = true;;

public:
	Camera();
	~Camera();

	void setScreenDimensions(float sWidth, float sHeight) {
		screenWidth = sWidth;
		screenHeight = sHeight;
		viewport.set(2, sWidth);
		viewport.set(3, sHeight);
		focus = Vector(screenWidth / 2, screenHeight / 2);
	}

	void setDisplay(Display * _display) {
		display = _display;
	}

	void setInput(Input * _input) {
		input = _input;
	}

	Matrix getViewport() {
		Matrix scaledPort = WorldMapper::scaleViewport(viewport, zoomFactor);
		return scaledPort;
	}

	void set(Vector _origin) {
		viewport = Matrix::translationMatrix(_origin);
	}

	void position(Vector _pos) {
		viewport.set(0, _pos.x);
		viewport.set(1, _pos.y);
	}

	void positionCenter(Vector centerPos) {
		viewport.set(0, centerPos.x - display->displayPort.get(2) / 2.0);
		viewport.set(1, centerPos.y - display->displayPort.get(3) / 2.0);
	}

	void move(Vector _delta) {
		viewport += Matrix::translationMatrix(_delta);
	}

	void lerp(Vector newOrigin, int frames);

	//Constant moving in a line
	void dolly(Vector delta);
	void stopDollying();

	void setZoom(float zoom) {
		if (zoom <= MAX_ZOOM && zoom >= MIN_ZOOM) {
			zoomFactor = zoom;
		}
	}

	void zoomIn(float delta = DEFAULT_ZOOM_SPEED) {
		setZoom(zoomFactor + delta);
	}

	void zoomOut(float delta = DEFAULT_ZOOM_SPEED) {
		setZoom(zoomFactor - delta);
	}

	void update();
};

