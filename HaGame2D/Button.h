#pragma once
#include "HaGame2D.h"
#include "ButtonComponent.h"

class Button
{

public:
	static GameObject * initialize(Scene * scene, Vector pos, Vector size, RGB color, std::function<void()> onClickFunc) {
		auto btn = new GameObject();
		btn->move(pos);
		btn->addComponent(new BoxRenderer(size.x, size.y, true, color));
		btn->addComponent(new ButtonComponent(size.x, size.y))->onClickFunc = onClickFunc;
		scene->instantiate(btn);
		return btn;
	}

	static GameObject * initialize(Scene * scene, Vector pos, Vector size, RGB color, std::string message, std::string font, int fontSize, RGB fontColor, std::function<void()> onClickFunc) {
		auto btn = new GameObject();
		btn->move(pos);
		btn->addComponent(new BoxRenderer(size.x, size.y, true))->color = color;
		btn->addComponent(new ButtonComponent(size.x, size.y))->onClickFunc = onClickFunc;
		auto label = btn->addComponent(new TextRenderer(size.x, size.y));
		label->setAllignment(TextAllignments::Center);
		label->setFont(font);
		label->setFontColor(fontColor);
		label->setFontSize(fontSize);
		label->setMessage(message);
		scene->instantiate(btn);
		return btn;
	}
};

class ButtonFactory {
	Scene *_scene;
	RGB _btnColor;
	RGB _fontColor;
	std::string _font;
	int _fontSize;

public:
	ButtonFactory(Scene *scene, RGB btnColor, RGB fontColor, int fontSize, std::string font) {
		_scene = scene;
		_btnColor = btnColor;
		_fontColor = fontColor;
		_fontSize = fontSize;
		_font = font;
	}

	GameObject * createEmptyButton(Vector pos, Vector size, std::function<void()> onClickFunc) {
		return Button::initialize(_scene, pos, size, _btnColor, onClickFunc);
	}

	GameObject * createTextButton(Vector pos, Vector size, std::string message, std::function<void()> onClickFunc) {
		return Button::initialize(_scene, pos, size, _btnColor, message, _font, _fontSize, _fontColor, onClickFunc);
	}
};
