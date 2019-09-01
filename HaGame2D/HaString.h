#pragma once
#include <vector>
#include <string>

class HaString
{
public:
	HaString();
	~HaString();

	static std::vector<std::string> str_split(std::string str, char delim) {
		std::vector<std::string> splitStr;
		std::string currentStr = "";

		int index = 0;
		char currentChar = str[0];

		while (currentChar != NULL) {
			if (currentChar == delim) {
				splitStr.push_back(currentStr);
				currentStr = "";
			}
			else {
				currentStr += currentChar;
			}
			index += 1;
			currentChar = str[index];
		}

		if (currentStr != "") {
			splitStr.push_back(currentStr);
		}

		return splitStr;
	}
};

