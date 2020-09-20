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

	double verticalMargin = 0;
	double horizontalMargin = 0;

	double textWidth;
	double textHeight;

	bool dirty;

	TextAllignments allignment = TextAllignments::Left;

public:

	Text text;

	TextRenderer(double _width, double _height);

	void setFontSize(int fs) {
		fontSize = fs;
		dirty = true;
	}

	void setFont(std::string _font) {
		font = _font;
		dirty = true;
	}

	void setFontColor(RGB fc) {
		fontColor = fc;
		dirty = true;
	}

	void setMessage(std::string _message) {
		message = _message;
		dirty = true;
	}

	void setMargin(double horiz, double vert) {
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

