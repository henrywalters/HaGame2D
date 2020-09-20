#pragma once
#include "Component.h"
#include "UIComponent.h"
#include "TextRenderer.h"

class ButtonComponent : public UIComponent
{

public:
	
	RGB color = Color::white();
	
	std::function<void()> onClickFunc;
	std::function<void()> onHoverFunc;

	ButtonComponent(double _width, double _height);
	~ButtonComponent();

	void onClick();
	void onHover();
	void updateUI();
};

