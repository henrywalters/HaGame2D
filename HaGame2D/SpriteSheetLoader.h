#pragma once
#include <vector>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <unordered_map>

struct SpriteSheetCell {
	std::string cellType;
	int index;
	double x, y, width, height;
	
	std::string getString() {
		std::string out = cellType + '_' + std::to_string(index) + '_' + std::to_string(x) + '_' + std::to_string(y) + '_' + std::to_string(width) + '_' + std::to_string(height);
		return out;
	}

	static SpriteSheetCell parseString(std::string cellString) {
		SpriteSheetCell cell;
		char delim = '_';

		int start = 0;
		int end = cellString.find(delim);
		
		std::string cellSubStr;

		int index = 0;

		cellSubStr = cellString.substr(start, end - start);
		cell.cellType = cellSubStr;
		start = end + 1;
		end = cellString.find(delim, start);

		cellSubStr = cellString.substr(start, end - start);
		cell.index = atoi(cellSubStr.c_str());
		start = end + 1;
		end = cellString.find(delim, start);

		cellSubStr = cellString.substr(start, end - start);
		cell.x = atof(cellSubStr.c_str());
		start = end + 1;
		end = cellString.find(delim, start);

		cellSubStr = cellString.substr(start, end - start);
		cell.y = atof(cellSubStr.c_str());
		start = end + 1;
		end = cellString.find(delim, start);

		cellSubStr = cellString.substr(start, end - start);
		cell.width = atof(cellSubStr.c_str());
		start = end + 1;
		end = cellString.find(delim, start);

		cellSubStr = cellString.substr(start, end - start);
		cell.height = atof(cellSubStr.c_str());
		start = end + 1;
		end = cellString.find(delim, start);

		return cell;
	}
};

class SpriteSheetLoader
{
public:
	SpriteSheetLoader();
	~SpriteSheetLoader();

	static bool save(std::string saveFile, std::vector<SpriteSheetCell> cells);
	static std::vector<SpriteSheetCell> load(std::string saveFile);

	static std::unordered_map < std::string, SpriteSheetCell > getSpriteMap(std::vector<SpriteSheetCell> cells);
	static std::unordered_map < std::string, SpriteSheetCell> getSpriteMap(std::string loadFile);
};

