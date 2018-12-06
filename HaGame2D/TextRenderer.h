#pragma once
#include "BoxComponent.h"
#include "Component.h"
#include "BoxComponent.h"
#include "GameObject.h"
#include "Color.h"
#include "Vector.h"
#include <string.h>

enum TextAllignments {
	Left,
	Center,
	Right,
	TopLeft,
	TopCenter,
	TopRight,
	BottomLeft,
	BottomCenter,
	BottomRight
};

class TextRenderer : public BoxComponent
{
private:

	int fontSize = DEFAULT_FONTSIZE;
	std::string font = DEFAULT_FONT;
	RGB fontColor = Color::white();
	std::string message = " ";

	float verticalMargin = 0;
	float horizontalMargin = 0;

	float textWidth;
	float textHeight;

	TextAllignments allignment = TextAllignments::Left;

public:

	Text text;

	TextRenderer(float _width, float _height);

	void setFontSize(int fs) {
		fontSize = fs;
	}

	void setFont(std::string _font) {
		font = _font;
	}

	void setFontColor(RGB fc) {
		fontColor = fc;
	}

	void setMessage(std::string _message) {
		message = _message;
	}

	void setMargin(float horiz, float vert) {
		verticalMargin = vert;
		horizontalMargin = horiz;
	}

	void setAllignment(TextAllignments _allignment) {
		allignment = _allignment;
	}

	~TextRenderer();

	void onCreate();
	void update();
};
