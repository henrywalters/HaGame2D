#pragma once
//#include "HaGame2D.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "HaString.h"
#include <sstream>

struct MapCell {
	float x;
	float y;
	float width;
	float height;
	int z_index;
	std::string key;

	friend std::ostream& operator<<(std::ostream& os, const MapCell& cell) {
		os << cell.x << "," << cell.y << "," << cell.width << "," << cell.height << "," << cell.z_index << "," << cell.key << "\n";
		return os;
	}

	friend std::istream& operator>>(std::istream& is, MapCell& cell) {
		std::string cellStr;
		is >> cellStr;
		
		//shorthand for cell
		auto c = HaString::str_split(cellStr, ',');

		cell.x = std::stof(c[0]);
		cell.y = std::stof(c[1]);
		cell.width = std::stof(c[2]);
		cell.height = std::stof(c[3]);
		cell.z_index = std::stoi(c[4]);
		cell.key = c[5];

		return is;
	}
};

class MapLoader
{
public:
	MapLoader();
	~MapLoader();

	static std::vector<MapCell> load(std::string filePath) {
		std::ifstream file(filePath);
		std::vector<MapCell> map;

		std::string cell;
		while (std::getline(file, cell)) {
			MapCell newCell;
			std::istringstream iss(cell);
			iss >> newCell;
			map.push_back(newCell);
		}
		file.close();
		
		return map;
	}

	static void save(std::string filePath, std::vector<MapCell> map) {
		std::ofstream ofs(filePath);
		for (auto cell : map) {
			ofs << cell;
		}
		ofs.close();
	}
};

