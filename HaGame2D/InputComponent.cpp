#include "stdafx.h"
#include "InputComponent.h"


InputComponent::InputComponent()
{
}


InputComponent::~InputComponent()
{
}

void InputComponent::onInput(std::string input)
{
	text += input;
	auto renderer = transform->getComponent<TextRenderer>();
	renderer->setMessage(text);
}

void InputComponent::updateUI()
{
	auto renderer = transform->getComponent<TextRenderer>();

	if (renderer != NULL) {
		renderer->setMessage(text);
	}
}
