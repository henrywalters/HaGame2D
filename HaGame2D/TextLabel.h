#pragma once
#include "GameObject.h"
#include "Scene.h"
#include "Component.h"
#include "TextRenderer.h"

class TextLabel {
public:
	static GameObject * initialize(Scene *scene, Vector pos, Vector size, std::string msg, std::string font, int fontSize, RGB color = Color::black()) {
		auto text = scene->add();
		text->setPosition(pos);
		auto renderer = text->addComponent(new TextRenderer(size.x, size.y));
		renderer->setAllignment(TextAllignments::Center);
		renderer->setFontSize(fontSize);
		renderer->setFontColor(color);
		renderer->setFont(font);
		renderer->setMessage(msg);
		return text;
	}
};


class TextLabelFactory {
	Scene *scene;
	std::string font;
	int fontSize;
	RGB color;

public:
	TextLabelFactory(Scene *_scene, std::string _font, int _fontSize, RGB _color) {
		scene = _scene;
		font = _font;
		fontSize = _fontSize;
		color = _color;
	};

	GameObject * build(std::string message, Vector pos, Vector size) {
		return TextLabel::initialize(scene, pos, size, message, font, fontSize, color);
	}
};