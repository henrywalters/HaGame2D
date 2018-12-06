#include "stdafx.h"
#include "TextRenderer.h"


TextRenderer::TextRenderer(float _width, float _height)
{
	width = _width; height = _height;
}


TextRenderer::~TextRenderer()
{
}

void TextRenderer::onCreate() {
	text = display->loadText(font, fontSize, message, fontColor);
	textWidth = text.size.x; 
	textHeight = text.size.y;
	printf("Text size: %f x %f", textWidth, textHeight);
}

void TextRenderer::update() {
	if (message.length() > 0) {

		float relativeWidthRatio = relativeWidth / width;
		float relativeHeightRatio = relativeHeight / height;

		textWidth = text.size.x;
		textHeight = text.size.y;

		float relTextWidth = textWidth * relativeWidthRatio;
		float relTextHeight = textHeight * relativeHeightRatio;

		float posX;
		float posY;

		switch (allignment) {
		case TextAllignments::Left:
			posX = transform->position.x;
			posY = transform->position.y + (height / 2 - textHeight / 2);
			break;
		case TextAllignments::Center:
			posX = transform->position.x + (width / 2 - textWidth / 2);
			posY = transform->position.y + (height / 2 - textHeight / 2);
			break;
		case TextAllignments::Right:
			posX = transform->position.x + width - textWidth;
			posY = transform->position.y + (height / 2 - textHeight / 2);
			break;
		case TextAllignments::TopLeft:
			posX = transform->position.x;
			posY = transform->position.y;
			break;
		case TextAllignments::TopCenter:
			posX = transform->position.x + (width / 2 - textWidth / 2);
			posY = transform->position.y;
			break;
		case TextAllignments::TopRight:
			posX = transform->position.x + width - textWidth;
			posY = transform->position.y;
			break;
		case TextAllignments::BottomLeft:
			posX = transform->position.x;
			posY = transform->position.y + width - textHeight;
			break;
		case TextAllignments::BottomCenter:
			posX = transform->position.x + (width / 2 - textWidth / 2);
			posY = transform->position.y + width - textHeight;
			break;
		case TextAllignments::BottomRight:
			posX = transform->position.x + width - textWidth;
			posY = transform->position.y + width - textHeight;
			break;
		}

		//printf("X = %f, Y = %f, Rel w = %f, h = %f\n", posX, transform->position.y, textWidth, textHeight);

		TextureRect rect = TextureRect{ posX, posY, textWidth, textHeight};
		TextureRect clip = TextureRect{ 0, 0, textWidth, textHeight };

		display->drawText(rect, text, clip, transform->z_index);
	}
}