#pragma once
#include "Component.h"
#include "BoxComponent.h"
#include "GameObject.h"
#include "SDL.h"

const int CLICK_DELAY = 100;

class UIComponent : public BoxComponent
{
	bool focused;
	bool canClick = true;
	int lastClick;
public:
	UIComponent();
	~UIComponent();

	virtual void onClick() {};
	virtual void onMouseUp() {};
	virtual void onMouseDown() {};
	virtual void onFocus() {};
	virtual void onBlur() {};
	virtual void onInput(std::string input) {};
	virtual void onFocusedInput(std::string input) {};

	virtual void updateUI() {};

	void update();
};

