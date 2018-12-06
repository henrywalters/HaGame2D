#include "stdafx.h"
#include "SpriteSheetLoader.h"


SpriteSheetLoader::SpriteSheetLoader()
{
}


SpriteSheetLoader::~SpriteSheetLoader()
{
}

bool SpriteSheetLoader::save(const char * saveFile, std::vector<SpriteSheetCell> cells) {
	std::ofstream file(saveFile);

	if (file.is_open()) {
		for (SpriteSheetCell cell : cells) {
			file << cell.getString() << "\n";
		}
		file.close();
		return true;
	}

	return false;
}

std::vector<SpriteSheetCell> SpriteSheetLoader::load(const char * loadFile) {
	std::ifstream file(loadFile);
	std::vector<SpriteSheetCell> cells;

	if (file.is_open()) {
		std::string line;
		while (std::getline(file, line)) {
			cells.push_back(SpriteSheetCell::parseString(line));
		}
	}

	return cells;
}

std::unordered_map<std::string, SpriteSheetCell> SpriteSheetLoader::getSpriteMap(std::vector<SpriteSheetCell> cells)
{
	auto animMap = std::unordered_map<std::string, SpriteSheetCell>();

	for (SpriteSheetCell cell : cells) {
		if (animMap.find(cell.cellType) == animMap.end()) {
			animMap[cell.cellType] = cell;
		}
	}

	return animMap;
}

std::unordered_map<std::string, SpriteSheetCell> SpriteSheetLoader::getSpriteMap(char * loadFile)
{
	return getSpriteMap(SpriteSheetLoader::load(loadFile));
}
