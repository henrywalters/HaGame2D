#include "stdafx.h"
#include "UIComponent.h"


UIComponent::UIComponent()
{
}


UIComponent::~UIComponent()
{
}

void UIComponent::update() {

	float mX = input->mouseXGlobal;
	float mY = input->mouseYGlobal;

	bool gotFocus = false;

	if (!canClick) {
		if (SDL_GetTicks() - lastClick >= CLICK_DELAY) {
			canClick = true;
		}
	}

	if (input->fire1) {
		if (mX >= transform->position.x && mX < transform->position.x + width && mY >= transform->position.y && mY < transform->position.y + height) {
				
			if (canClick) {
				onClick();
				lastClick = SDL_GetTicks();
				canClick = false;
			}

			gotFocus = true;
		}
	}

	if (input->fire1Down) {
		if (mX >= transform->position.x && mX < transform->position.x + width && mY >= transform->position.y && mY < transform->position.y + height) {
			onMouseDown();
		}
	}

	if (input->fire1Up) {
		if (mX >= transform->position.x && mX < transform->position.x + width && mY >= transform->position.y && mY < transform->position.y + height) {
			onMouseUp();
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