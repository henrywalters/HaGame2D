#include "stdafx.h"
#include "Display.h"
#include <iostream>
#include <filesystem>

Display::Display(int w, int h, const char * _title) : width(w), height(h), title(_title)
{

	displayPort = Matrix(Vector(0, 0), Vector(w, h));

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC );
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN);
	screen = SDL_GetWindowSurface(window);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED  );

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
	}

	if (TTF_Init() == -1) {
		printf("SDL_ttf failed to initialize with error %s", SDL_GetError());
		exit(2);
	}

	for (int i = Z_INDEX_START; i < Z_INDEX_END; i++) {
		displayQueue[i] = std::queue<std::function<void()>>();
	}

}

void Display::initialize(SDL_Window * _window, SDL_Surface * _screen, SDL_Renderer * _renderer) {
	window = _window;
	screen = _screen;
	renderer = _renderer;
}

Display::Display() {

}

Display::~Display()
{
}

void Display::setDisplayPort(double x, double y, double w, double h) {
	displayPort = Matrix(Vector(x, y), Vector(w,h));
}

void Display::clear() {
	SDL_SetRenderDrawColor(renderer, clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	SDL_RenderClear(renderer);
}

void adjustRect(SDL_Rect &rect, Vector displace) {
	rect.x -= displace.x;
	rect.y -= displace.y;
}

void Display::draw() {

	SDL_Rect port;
	port.x = displayPort.get(0);
	port.y = displayPort.get(1);
	port.w = displayPort.get(2);
	port.h = displayPort.get(3);

	SDL_RenderSetViewport(renderer, &port);
	
	for (int i = Z_INDEX_START; i < Z_INDEX_END; i++) {
		if (displayQueue[i].size() > 0) {
			while (!displayQueue[i].empty()) {
				auto func = displayQueue[i].front();
				func();
				displayQueue[i].pop();
			}
		}
	}
}

void Display::render() {
	SDL_RenderPresent(renderer);
}

void Display::displayMetric(std::string metric)
{
	std::string newTitle = std::string(title) + " - " + metric;
	SDL_SetWindowTitle(window, newTitle.c_str());
}

void Display::setRenderColor(RGB rgb, int alpha) {
	SDL_SetRenderDrawColor(renderer, rgb.r, rgb.g, rgb.b, rgb.a);
}

void Display::drawPixel(double x, double y, RGB color, int z_index)
{
	dispatch(z_index, [this, x, y, color] {
		setRenderColor(color);
		SDL_RenderDrawLine(renderer, x, y, x, y);
	});
}

void Display::drawPixel(Vector pos, RGB color, int z_index)
{
	drawPixel(pos.x, pos.y, color, z_index);
}

void Display::drawRect(double x, double y, double _width, double _height, RGB color, int z_index) {
	if (!inDisplayPort(x, y, _width, _height)) return;
	SDL_Rect rect = { x, y, _width, _height };
	dispatch(z_index, [this, rect, color] {
		setRenderColor(color);
		SDL_RenderDrawRect(renderer, &rect); 
	});
}

void Display::fillRect(double x, double y, double _width, double _height, RGB color, int z_index) {
	if (!inDisplayPort(x, y, _width, _height)) return;
	SDL_Rect rect = { x, y, _width, _height };
	dispatch(z_index, [this, rect, color] { 
		setRenderColor(color, 5);
		SDL_RenderFillRect(renderer, &rect); 
	});
}

void Display::drawLine(double x1, double y1, double x2, double y2, RGB color, int z_index) {
	if (!inDisplayPort(Vector(x1, y1), Vector(x2, y2))) return;
	dispatch(z_index, [this, x1, y1, x2, y2, color] {
		setRenderColor(color);
		SDL_RenderDrawLine(renderer, x1, y1, x2, y2); 
	});
}

void Display::drawLine(Vector p1, Vector p2, RGB color, int z_index)
{
	drawLine(p1.x, p1.y, p2.x, p2.y, color, z_index);
}

void Display::drawTriangle(Vector p1, Vector p2, Vector p3, RGB color, int z_index)
{
	Vector center = Vector(displayPort.get(2) / 2, displayPort.get(3) / 2);

	Vector p1Prime = p1 + center;
	Vector p2Prime = p2 + center;
	Vector p3Prime = p3 + center;

	dispatch(z_index, [this, p1Prime, p2Prime, p3Prime, color, z_index]() {
		drawLine(p1Prime, p2Prime, color, z_index);
		drawLine(p2Prime, p3Prime, color, z_index);
		drawLine(p3Prime, p1Prime, color, z_index);
	});
	
}

void Display::drawTriangle(Triangle triangle, RGB color, int z_index)
{
	drawTriangle(triangle.p1, triangle.p2, triangle.p3, color, z_index);
}

void Display::drawPNG(PNG png) {

	SDL_BlitSurface(png.image, NULL, screen, NULL);
}

void Display::drawTexture(TextureRect rect, Texture texture, TextureRect clip, int z_index) {
	if (!inDisplayPort(rect)) return;
	SDL_Rect sdlRect = rect.getSDLRect();
	SDL_Rect *sdlClip = clip.getSDLRectPointer();
	dispatch(z_index, [this, texture, sdlClip, sdlRect] { 
		SDL_Point* point = new SDL_Point();
		point->x = texture.anchor.x;
		point->y = texture.anchor.y;
		SDL_SetTextureAlphaMod(texture.texture, texture.alpha);
		SDL_RendererFlip flip = texture.angle > 90 && texture.angle < 270 ? SDL_FLIP_VERTICAL : SDL_FLIP_NONE;
		SDL_RenderCopyEx(renderer, texture.texture, sdlClip, &sdlRect, texture.angle * (180 / M_PI), point, flip);
		free(sdlClip);
	});
}

void Display::drawText(TextureRect rect, Text text, TextureRect clip, int z_index) {
	if (!inDisplayPort(rect)) return;
	SDL_Rect sdlRect = rect.getSDLRect();
	SDL_Rect *sdlClip = clip.getSDLRectPointer();
	dispatch(z_index, [this, text, z_index, sdlClip, sdlRect] {
		SDL_RenderCopy(renderer, text.texture, sdlClip, &sdlRect);
		free(sdlClip);
	});
}

PNG Display::loadPNG(std::string path) {
	PNG png = PNG{ path, NULL };
	SDL_Surface * loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s SDL ERROR: %s\n", path.c_str(), IMG_GetError());
	}
	else {
		if (screen == NULL) {
			std::cout << "Screen is null\n";
		}
		png.image = SDL_ConvertSurface(loadedSurface, screen->format, NULL);
		if (png.image == NULL)
		{
			printf("Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		SDL_FreeSurface(loadedSurface);
	}
	return png;
}

Texture Display::loadTexture(std::string path) {
	//The final texture
	Texture newTexture = Texture{ 0xFF, path, NULL };

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture.texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture.texture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else {
			SDL_SetTextureBlendMode(newTexture.texture, SDL_BLENDMODE_BLEND);
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

Text Display::loadText(std::string font, int fontSize, std::string text, RGB color) {
	Text newText;
	TTF_Font *ttf = TTF_OpenFont(font.c_str(), fontSize);
	if (ttf != NULL) {
		newText.fontName = font;
		newText.font = ttf;
		newText.fontSize = fontSize;
		newText.text = text;

		SDL_Color textColor = { color.r, color.g, color.b };

		SDL_Surface * textSurface = TTF_RenderText_Solid(newText.font, text.c_str(), textColor);

		if (textSurface == NULL || renderer == NULL) {
			printf("Something went wrong creating surface for %s because of %s", text.c_str(), SDL_GetError());
		}
		else {
			newText.texture = SDL_CreateTextureFromSurface(renderer, textSurface);
			newText.size = Vector(textSurface->w, textSurface->h);

			if (newText.texture == NULL) {
				printf("Something went wrong printing %s because of %s", text.c_str(), SDL_GetError());
			}
		}

		SDL_FreeSurface(textSurface);
	}
	else {
		printf("Failed to print text because of %s", SDL_GetError());
	}

	return newText;
	
}

void Display::clearText(Text textObject) {
	SDL_DestroyTexture(textObject.texture);
}

void Display::setFont(int fontSize, std::string path) {
	font = TTF_OpenFont(path.c_str(), fontSize);
	if (font == NULL) {
		printf("Failed to load font: %s with error: %s", font, TTF_GetError());
	}
}