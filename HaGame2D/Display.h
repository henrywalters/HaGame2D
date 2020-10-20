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
#include "Polygon.h"

struct PNG {
	std::string path;
	SDL_Surface * image;
};

struct Triangle {
	Vector p1;
	Vector p2;
	Vector p3;

	std::string toString() {
		return p1.toString() + ", " + p2.toString() + ", " + p3.toString();
	}

	Triangle operator + (Vector vector) {
		return Triangle{
			p1 + vector,
			p2 + vector,
			p3 + vector
		};
	}
};

struct Texture {
	Uint8 alpha;
	std::string path;
	SDL_Texture * texture;
	Vector anchor;
	double angle;
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
	double x, y, width, height;

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
const int Z_INDEX_END = 10000;

const int Z_DEFAULT = 10;

const std::string DOROVAR_FONT = "DorovarFLF-Carolus.ttf";
const std::string DOROVAR_ITALIC_FONT = "DOROVARFLF_Italic.ttf";
const std::string BLOODLUST_FONT = "bloodlust.ttf";

const std::string NEUROPOL = "neuropol-x-free.regular.ttf";

const std::string DEFAULT_FONT = NEUROPOL;
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

	RGB clearColor = Color::black();

	//Call this function from the sub scene so that it may inhereit the render properties without actually creating a new display window.
	void initialize(SDL_Window * window, SDL_Surface * screen, SDL_Renderer * renderer);

	int width;
	int height;
	const char * title;

	Display();
	Display(int _width, int _height, const char* _title = "HaGame2D - Untitled Game");
	~Display();

	void setDisplayPort(double x, double y, double width, double height);

	static Vector monitorSize() {
		SDL_DisplayMode DM;
		SDL_GetCurrentDisplayMode(0, &DM);
		return Vector(DM.w, DM.h);
	}

	inline bool inScreen(double x, double y) {
		std::cout << "Size: " << width << "x" << height << std::endl;
		return x >= 0 && x <= width && y >= 0 && y <= height;
	}

	inline bool inScreen(Vector v) {
		return inScreen(v.x, v.y);
	}

	void clear();
	void draw();
	void render();

	void displayMetric(std::string);

	void setRenderColor(RGB rgb, int alpha = 0xFF);
	void drawPixel(double x, double y, RGB color, int z_index = Z_DEFAULT);
	void drawPixel(Vector pos, RGB color, int z_index = Z_DEFAULT);
	void drawRect(double x, double y, double _width, double _height, RGB color, int z_index = Z_DEFAULT);
	void fillRect(double x, double y, double _width, double _height, RGB color, int z_index = Z_DEFAULT);
	void drawLine(double x1, double y1, double x2, double y2, RGB color, int z_index = Z_DEFAULT);
	void drawLine(Vector p1, Vector p2, RGB color, int z_index = Z_DEFAULT);

	template <size_t size>
	void drawPolygon(Polygon<size> polygon, RGB color, int z_index = Z_DEFAULT);

	void drawTriangle(Vector p1, Vector p2, Vector p3, RGB color, int z_index = Z_DEFAULT);
	void drawTriangle(Triangle triangle, RGB color, int z_index = Z_DEFAULT);
	void drawPNG(PNG png);
	void drawTexture(TextureRect rect, Texture texture, TextureRect clip, int z_index = Z_DEFAULT);

	void Display::drawText(TextureRect rect, Text text, TextureRect clip, int z_index);

	PNG loadPNG(std::string path);
	Texture loadTexture(std::string path);
	void setFont(int fontSize, std::string font = DEFAULT_FONT);

	void destroyText(Text text) {
		SDL_DestroyTexture(text.texture);
		TTF_CloseFont(text.font);
	}

	Text loadText(std::string font, int fontSize, std::string text, RGB color = Color::white());


	//Helpful because this function will update the size of the text object for you.
	//Text updateText(Text textObject, std::string text, int fontSize = DEFAULT_FONTSIZE, RGB color = Color::white());

	void clearText(Text textObject);

	bool inDisplayPort(Vector point) {
		return (point.x >= displayPort.get(0) && point.x < displayPort.get(0) + displayPort.get(2) && point.y >= displayPort.get(1) && point.y < displayPort.get(1) + displayPort.get(3));
	}

	bool inDisplayPort(Vector size, Vector pos) {
		return inDisplayPort(pos) || inDisplayPort(pos + size.xVec()) || inDisplayPort(pos + size.yVec()) || inDisplayPort(pos + size);
	}

	bool inDisplayPort(double x, double y, double _width, double _height) {
		return inDisplayPort(Vector(_width, _height), Vector(x, y));
	}

	bool inDisplayPort(TextureRect rect) {
		return inDisplayPort(rect.x, rect.y, rect.width, rect.height);
	}

	bool inDisplayPort(Line line) {
		return inDisplayPort(line.p1()) || inDisplayPort(line.p2());
	}

};

template<size_t size>
inline void Display::drawPolygon(Polygon<size> polygon, RGB color, int z_index)
{
	if (size <= 0) {
		throw new std::exception("Invalid Polygon size");
	}
	else if (size == 1) {
		drawPixel(polygon.vertices[0], color, z_index);
	}
	else {
		for (int i = 1; i < size; i++) {
			drawLine(polygon.vertices[i - 1], polygon.vertices[i], color, z_index);
		}
	}
}
