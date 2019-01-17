#pragma once
#include <string>

class SpriteSheetHelper
{
public:
	SpriteSheetHelper(std::string spritePath, int width, int height);

	static void autoLoad(int width, int height, int rows, int cols, char * saveTo, std::string rowNames[], std::string colNames[]);

	~SpriteSheetHelper();
};

