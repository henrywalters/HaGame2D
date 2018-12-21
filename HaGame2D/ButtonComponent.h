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

	ButtonComponent(float _width, float _height);
	~ButtonComponent();

	void onClick();
	void onHover();
	void updateUI();
};

