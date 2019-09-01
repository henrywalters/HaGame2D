#pragma once
#include "SDL.h"
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string.h>
#include "Color.h"
#include <string>
#include <queue>
#include <functional>
#include <iostream>
#include "Vector.h"
#include "Matrix.h"

struct PNG {
	std::string path;
	SDL_Surface * image;
};

struct Texture {
	std::string path;
	SDL_Texture * texture;
	float angle;
	Vector anchor;
};

struct Text {
	int fontSize;
	std::string text;
	Vector size;
	TTF_Font * font;
	std::string fontName;
	SDL_Texture * texture;
	RGB textColor;
};

struct TextureRect {
	float x, y, width, height;

	SDL_Rect * getSDLRectPointer() {
		SDL_Rect * rect = new SDL_Rect;
		rect->x = x;
		rect->y = y;
		rect->w = width;
		rect->h = height;
		return rect;
	}

	SDL_Rect getSDLRect() {
		SDL_Rect rect;
		rect.x = x;
		rect.y = y;
		rect.w = width;
		rect.h = height;
		return rect;
	}

};

const int Z_INDEX_START = 0;
const int Z_INDEX_END = 20;

const int Z_DEFAULT = 10;

const std::string DOROVAR_FONT = "DorovarFLF-Carolus.ttf";
const std::string DOROVAR_ITALIC_FONT = "DOROVARFLF_Italic.ttf";
const std::string BLOODLUST_FONT = "bloodlust.ttf";

const std::string DEFAULT_FONT = DOROVAR_FONT;
const int DEFAULT_FONTSIZE = 24;

class Display
{
	std::queue<std::function<void()>> displayQueue[Z_INDEX_END - Z_INDEX_START];

	void dispatch(int z_index, std::function<void()> drawFunction) {
		if (z_index >= Z_INDEX_START && z_index < Z_INDEX_END) {
			displayQueue[z_index].push(drawFunction);
		}
		else {
			std::cout << "WARNING - Dispatching at z-index: " << z_index << " range is " << Z_INDEX_START << " - " << Z_INDEX_END << "\n";
		}
	}

public:
	Matrix displayPort;
	SDL_Window * window = NULL;
	SDL_Surface * screen = NULL;
	SDL_Renderer * renderer = NULL;
	TTF_Font *font = NULL;

	//Call this function from the sub scene so that it may inhereit the render properties without actually creating a new display window.
	void initialize(SDL_Window * window, SDL_Surface * screen, SDL_Renderer * renderer);

	int width;
	int height;
	char * title;

	Display();
	Display(int width, int height, char * title = "HaGame2D - Untitled Game");
	~Display();

	void setDisplayPort(float x, float y, float width, float height);

	void clear();
	void draw();
	void render();

	void setRenderColor(RGB rgb, int alpha = 0xFF);
	void drawRect(float x, float y, float width, float height, RGB color, int z_index = Z_DEFAULT);
	void fillRect(float x, float y, float width, float height, RGB color, int z_index = Z_DEFAULT);
	void drawLine(float x1, float y1, float x2, float y2, RGB color, int z_index = Z_DEFAULT);
	void drawPNG(PNG png);
	void drawTexture(TextureRect rect, Texture texture, TextureRect clip, int z_index = Z_DEFAULT);

	void Display::drawText(TextureRect rect, Text text, TextureRect clip, int z_index);

	PNG loadPNG(std::string path);
	Texture loadTexture(std::string path);
	void setFont(int fontSize, std::string font = DEFAULT_FONT);

	Text loadText(std::string font, int fontSize, std::string text, RGB color = Color::white());

	//Helpful because this function will update the size of the text object for you.
	//Text updateText(Text textObject, std::string text, int fontSize = DEFAULT_FONTSIZE, RGB color = Color::white());

	void clearText(Text textObject);

	bool inDisplayPort(Vector point) {
		return (point.x >= displayPort.get(0) && point.x < displayPort.get(0) + displayPort.get(2) && point.y >= displayPort.get(1) && point.y < displayPort.get(1) + displayPort.get(3));
	}

};

