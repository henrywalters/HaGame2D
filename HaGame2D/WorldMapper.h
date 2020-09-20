#pragma once
#include "Vector.h"
#include "Matrix.h"

class WorldMapper
{
public:
	WorldMapper();
	~WorldMapper();

	static Matrix scaleViewport(Matrix viewport, double scale);
	static Vector getScale(Matrix viewport, Matrix screen);

	static Matrix worldToViewport(Matrix worldObj, Matrix viewport);
	static Matrix viewportToScreen(Matrix viewportObj, Matrix viewport, Matrix screen);
	static Matrix worldToScreen(Matrix worldObj, Matrix viewport, Matrix screen);

	static Matrix screenToViewport(Matrix screenObj, Matrix screen, Matrix viewport);
	static Matrix viewportToWorld(Matrix viewportObj, Matrix viewport);
	static Matrix screenToWorld(Matrix screenObj, Matrix screen, Matrix viewport);
};

