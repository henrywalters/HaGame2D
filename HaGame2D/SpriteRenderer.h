#pragma once
#include "HaGame2D.h"
#include "SpriteSheetLoader.h"
#include <string>


class SpriteRenderer :
	public BoxComponent
{
	TextureRect clip;
	bool initialized = false;
public:

	std::string path;
	Texture sprite;
	
	//Render a sprite from a sprite sheet with config file.
	SpriteRenderer(Texture _spriteSheet, SpriteSheetCell _sprite, float _width, float _height) {
		sprite = _spriteSheet;
		width = _width;
		height = _height;
		clip = TextureRect{ _sprite.x, _sprite.y, _sprite.width, _sprite.height };
		initialized = true;
	}

	void setSpriteSheetCell(SpriteSheetCell _sprite) {
		clip = TextureRect{ _sprite.x, _sprite.y, _sprite.width, _sprite.height };
	}

	//Render a sprite - Null clip will assume to clip to 0,0,width,height
	SpriteRenderer(std::string _path, float _width, float _height, TextureRect *_clip) {
		path = _path;
		width = _width;
		height = _height;

		if (_clip == NULL) {
			clip = * new TextureRect{ 0, 0, width, height };
		}
		else {
			clip = *_clip;
		}
	}
	~SpriteRenderer();

	void onCreate() {
		if (!initialized) {
			sprite = display->loadTexture(path);
		}
	}

	void update() {
		display->drawTexture(TextureRect{ transform->relativePosition.x, transform->relativePosition.y, width, height }, sprite, clip, transform->z_index);
	}
};

