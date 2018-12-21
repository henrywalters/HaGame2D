#pragma once
class SpriteSheetHelper
{
public:
	SpriteSheetHelper(int width, int height);

	static void autoLoad(int width, int height, int rows, int cols, char * saveTo);

	~SpriteSheetHelper();
};

