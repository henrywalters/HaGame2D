#include "stdafx.h"
#include "UIComponent.h"


UIComponent::UIComponent()
{
}


UIComponent::~UIComponent()
{
}

void UIComponent::update() {

	double mX = input->mouseXGlobal;
	double mY = input->mouseYGlobal;

	bool gotFocus = false;
	/*
	if (!canClick) {
		if (SDL_GetTicks() - lastClick >= CLICK_DELAY) {
			canClick = true;
		}
	}
	*/
	if (mX >= transform->position.x && mX < transform->position.x + width && mY >= transform->position.y && mY < transform->position.y + height) {
		//onHover();
		/*
		if (input->fire1) {
			if (canClick) {
				//onClick();
				lastClick = SDL_GetTicks();
				canClick = false;
			}

			gotF
		*/

		if (input->fire1Down && canClick) {
			onClick();
			onMouseDown();
			canClick = false;
		}

		if (input->fire1Up) {
			onMouseUp();
			canClick = true;
		}

	}



	if (gotFocus && !focused) {
		focused = true;
		onFocus();
	}

	if (focused && !gotFocus) {
		focused = false;
		onBlur();
	}

	if (input->streamingText) {
		std::string inputStream = input->currentTextStream;
		onInput(inputStream);

		if (focused) {
			onFocusedInput(inputStream);
		}
	}

	updateUI();
}