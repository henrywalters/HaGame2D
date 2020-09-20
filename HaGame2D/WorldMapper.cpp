#include "stdafx.h"
#include "WorldMapper.h"


WorldMapper::WorldMapper()
{
}

Matrix WorldMapper::scaleViewport(Matrix viewport, double scale) {
	//std::cout << "Unscaled Viewport: " << viewport.toString();
	Matrix scaledViewport = Matrix::scaleMatrix(scale) * viewport;
	scaledViewport.set(0, scaledViewport.get(0) + 0.5 * (scaledViewport.get(0) - viewport.get(0)));
	scaledViewport.set(1, scaledViewport.get(1) + 0.5 * (scaledViewport.get(1) - viewport.get(1)));
	//Matrix delta = scaledViewport - viewport;
	//std::cout << "Scaled Viewport: " << finalViewport.toString();
	return scaledViewport;
}

Vector WorldMapper::getScale(Matrix viewport, Matrix screen) {
	return Vector(screen.get(0, 1) / viewport.get(0, 1), screen.get(1, 1) / viewport.get(1, 1));
}

Matrix WorldMapper::worldToViewport(Matrix worldObj, Matrix viewport) {
	return worldObj - Matrix::translationMatrix(viewport);
}

Matrix WorldMapper::viewportToScreen(Matrix viewportObj, Matrix viewport, Matrix screen) {
	//scale of screen / view broken into x & y components
	Vector scale = WorldMapper::getScale(viewport, screen);
	return Matrix::scaleMatrix(scale.x, scale.y) * viewportObj;
}

Matrix WorldMapper::worldToScreen(Matrix worldObj, Matrix viewport, Matrix screen) {
	return WorldMapper::viewportToScreen(WorldMapper::worldToViewport(worldObj, viewport), viewport, screen);
}

Matrix WorldMapper::screenToViewport(Matrix screenObject, Matrix screen, Matrix viewport) {
	return Matrix::scaleMatrix(viewport.get(0, 1) / screen.get(0, 1), viewport.get(1, 1) / screen.get(1, 1)) * screenObject;
}

Matrix WorldMapper::viewportToWorld(Matrix viewportObj, Matrix viewport) {
	return viewportObj + Matrix::translationMatrix(viewport);
}

Matrix WorldMapper::screenToWorld(Matrix screenObj, Matrix screen, Matrix viewport) {
	return WorldMapper::viewportToWorld(WorldMapper::screenToViewport(screenObj, screen, viewport), viewport);
}

WorldMapper::~WorldMapper()
{
}
