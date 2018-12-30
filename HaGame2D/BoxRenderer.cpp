#include "stdafx.h"
#include "BoxRenderer.h"


BoxRenderer::BoxRenderer(float _width, float _height)
{
	width = _width;
	height = _height;
}


BoxRenderer::~BoxRenderer()
{
}

void BoxRenderer::update() {
	if (fill) {
		display->fillRect(transform->relativePosition.x, transform->relativePosition.y, width, height, color, transform->z_index);
	}
	else {
		display->drawRect(transform->relativePosition.x, transform->relativePosition.y, width, height, color, transform->z_index);
	}
}