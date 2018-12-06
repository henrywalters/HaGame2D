#pragma once
#include "Component.h"
#include "Input.h"
#include "GameObject.h"
#include "TextRenderer.h"
#include "UIComponent.h"

class InputComponent : public UIComponent
{
public:

	std::string text;

	InputComponent();
	~InputComponent();

	void onInput(std::string input);
	void updateUI();
};

