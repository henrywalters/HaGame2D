#include "stdafx.h"
#include "TextRenderer.h"


TextRenderer::TextRenderer(double _width, double _height)
{
	width = _width; height = _height;
}


TextRenderer::~TextRenderer()
{
}

void TextRenderer::onCreate() {
	dirty = false;
	text = display->loadText(font, fontSize, message, fontColor);
	textWidth = text.size.x; 
	textHeight = text.size.y;
}

void TextRenderer::update() {
	if (message.length() > 0) {

		if (dirty) {
			display->destroyText(text);
			text = display->loadText(font, fontSize, message, fontColor);
			textWidth = text.size.x;
			textHeight = text.size.y;

			dirty = false;
		}

		double relativeWidthRatio = relativeWidth / width;
		double relativeHeightRatio = relativeHeight / height;

		textWidth = text.size.x;
		textHeight = text.size.y;

		double relTextWidth = textWidth * relativeWidthRatio;
		double relTextHeight = textHeight * relativeHeightRatio;

		double posX;
		double posY;

		switch (allignment) {
		case TextAllignments::Left:
			posX = transform->relativePosition.x;
			posY = transform->relativePosition.y + (height / 2 - textHeight / 2);
			break;
		case TextAllignments::Center:
			posX = transform->relativePosition.x + (width / 2 - textWidth / 2);
			posY = transform->relativePosition.y + (height / 2 - textHeight / 2);
			break;
		case TextAllignments::Right:
			posX = transform->relativePosition.x + width - textWidth;
			posY = transform->relativePosition.y + (height / 2 - textHeight / 2);
			break;
		case TextAllignments::TopLeft:
			posX = transform->relativePosition.x;
			posY = transform->relativePosition.y;
			break;
		case TextAllignments::TopCenter:
			posX = transform->relativePosition.x + (width / 2 - textWidth / 2);
			posY = transform->relativePosition.y;
			break;
		case TextAllignments::TopRight:
			posX = transform->relativePosition.x + width - textWidth;
			posY = transform->relativePosition.y;
			break;
		case TextAllignments::BottomLeft:
			posX = transform->relativePosition.x;
			posY = transform->relativePosition.y + width - textHeight;
			break;
		case TextAllignments::BottomCenter:
			posX = transform->relativePosition.x + (width / 2 - textWidth / 2);
			posY = transform->relativePosition.y + width - textHeight;
			break;
		case TextAllignments::BottomRight:
			posX = transform->relativePosition.x + width - textWidth;
			posY = transform->relativePosition.y + width - textHeight;
			break;
		}

		//printf("X = %f, Y = %f, Rel w = %f, h = %f\n", posX, transform->position.y, textWidth, textHeight);

		TextureRect rect = TextureRect{ posX, posY, textWidth, textHeight};
		TextureRect clip = TextureRect{ 0, 0, textWidth, textHeight };

		display->drawText(rect, text, clip, transform->z_index);
	}
}