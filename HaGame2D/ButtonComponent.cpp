#include "stdafx.h"
#include "ButtonComponent.h"


ButtonComponent::ButtonComponent(double _width, double _height)
{
	width = _width; height = _height;
}


ButtonComponent::~ButtonComponent()
{

}

void ButtonComponent::onClick() {
	onClickFunc();
}

void ButtonComponent::onHover() {
	onHoverFunc();
}

void ButtonComponent::updateUI() {
	
	display->fillRect(transform->relativePosition.x, transform->relativePosition.y, relativeWidth, relativeHeight, color, 5);
}