#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <unordered_map>

const std::string PARAMETER_DELIMITER = ",";
const std::string PARAMETER_START = "";
const std::string PARAMETER_END = "\n";

enum ParameterType {
	Boolean,
	Float,
	Integer,
	String
};

const std::string PARAMETER_TYPES[] = {
	"float",
	"bool",
	"int",
	"string"
};


/*
struct Parameter {
	ParameterType type;
	std::string key;
	std::string value;

	std::string toString() {
		return PARAMETER_TYPES[type] + " " + key + " = " + value;
	}

	template <class T>
	T getValue() {

		T outputVal;

		switch (type) {
		case ParameterType::Boolean:
			if (std::is_same<T, bool>::value) outputVal = value == "true" || value == "t" || value == "True" || value == "1" ? true : false;
			break;
		case ParameterType::Float:
			if (std::is_same<T, float>::value) outputVal = stof(value);
			break;
		case ParameterType::Integer:
			if (std::is_same<T, int>::value) outputVal = stoi(value);
			break;
		case ParameterType::String:
			if (std::is_same < T, std::string>::value) outputVal = value;
			break;
		default:
			std::cout << "WARNING - either type is not set or incorrectly calling the value.";
			break;
		}

		return dynamic_cast<T>(outputVal);
	}

	template <class T>
	bool typeMatch() {
		switch (type) {
		case ParameterType::Boolean && std::is_same<T, bool>::value:
			return true;
			break;
		case ParameterType::Float && std::is_same<T, float>::value:
			return true;
			break;
		case ParameterType::Integer && std::is_same<T, int>::value:
			return true;
			break;
		case ParameterType::String && std::is_same < T, std::string>::value:
			return true;
			break;
		default:
			return false;
			break;
		}
	}
};


*/


//Class to help load parameters into and out of a file.
/*
class ParameterSet
{
	std::unordered_map<std::string, Parameter> set;
	std::vector<std::string> keys;

public:
	ParameterSet();
	~ParameterSet();

	void addString(std::string key, std::string value) {
		if (set.find(key) == set.end()) {
			Parameter param;
			param.key = key;
			param.value = value;
			param.type = ParameterType::String;
			set[key] = param;
			keys.push_back(key);
		}
		else {
			set[key].value = value;
			set[key].type = ParameterType::String;
		}
	}

	void addBool(std::string key, bool value) {
		if (set.find(key) == set.end()) {
			Parameter param;
			param.key = key;
			param.value = value ? "true" : "false";
			param.type = ParameterType::Boolean;
			set[key] = param;
			keys.push_back(key);
		}
		else {
			set[key].value = value;
			set[key].type = ParameterType::Boolean;
		}
	}

	void addInt(std::string key, int value) {
		if (set.find(key) == set.end()) {
			Parameter param;
			param.key = key;
			param.value = std::to_string(value);
			param.type = ParameterType::Integer;
			set[key] = param;
			keys.push_back(key);
		}
		else {
			set[key].value = value;
			set[key].type = ParameterType::Integer;
		}
	}

	void addFloat(std::string key, float value) {
		if (set.find(key) == set.end()) {
			Parameter param;
			param.key = key;
			param.value = std::to_string(value);
			param.type = ParameterType::Float;
			set[key] = param;
			keys.push_back(key);
		}
		else {
			set[key].value = value;
			set[key].type = ParameterType::Float;
		}
	}

	void addParameter(Parameter param) {
		if (set.find(param.key) == set.end()) {
			keys.push_back(param.key);
		}
		set[param.key] = param;
		
	}

	template <class T>
	void setValue(std::string key, T value) {
		if (set.find(key) == set.end()) {
			std::cout << "WARNING - value does not exist for key: " << key << "\n";
		} 
		else if (!set[key].typeMatch<T>()) {
			std::cout << "WARNING - type mismatch. Expected: " << PARAMETER_TYPES[set[key].type] << " but got " << typeid(T).name() << " instead.\n";
		}
		else {
			switch (set[key].type) {
			case ParameterType::Boolean:
				addBool(key, value);
				break;
			case ParameterType::Float:
				addFloat(key, value);
				break;
			case ParameterType::Integer:
				addInt(key, value);
				break;
			case ParameterType::String:
				addString(key, value);
				break;
			default:
				std::cout << "WARNING - error setting value";
			}
		}
	}

	template <class T>
	T get(std::string key) {
		if (set.find(key) != set.end()) {
			return set[key].getValue<T>();
		}
		else {
			std::cout << "WARNING - value does not exist for key: " << key << "\n";
		}
	}

	std::string toString() {
		std::string output = PARAMETER_START;
		for (auto iter = keys.begin(); iter != keys.end(); ++iter) {
			output += set[*iter].toString();
			if (std::next(iter) != keys.end()) {
				output += PARAMETER_DELIMITER;
			}
			else {
				output += PARAMETER_END;
			}
		}
		return output;
	}
};

class ParameterSetList {
	
	std::vector<ParameterSet> list;

	std::vector<std::string> split(std::string str, char delim) {
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

public:

	ParameterSetList() {};
	~ParameterSetList() {};

	void addSet(ParameterSet set) {
		list.push_back(set);
	}

	ParameterSet getSet(int index) {
		return list[index];
	}

	std::vector<ParameterSet> getList() {
		return list;
	}

	void clearList() {
		list.clear();
	}

	std::string toString() {
		std::string output = "";
		for (ParameterSet set : list) {
			output += set.toString();
		}
		return output;
	}

	void save(char * filePath) {
		std::ofstream file(filePath);
		if (file.is_open()) {
			file << toString();
			file.close();
		}
		else {
			std::cout << "WARNING - failed to save to path: " << filePath << "\n";
		}
	}

	void load(char * filePath) {
		clearList();
		std::ifstream file(filePath);

		if (file.is_open()) {
			std::string set;
			while (std::getline(file, set)) {
				ParameterSet parsedSet = parse(set);
				addSet(parsedSet);
			}
		}
		else {
			std::cout << "WARNING - failed to open path: " << filePath << "\n";
		}
	}

	ParameterSet parse(std::string set) {
		ParameterSet paramSet;
		auto parameters = split(set, ',');
		for (std::string parameter : parameters) {
			auto subParam = split(parameter, ' ');
			std::string type = subParam[0];

			Parameter param;
			param.key = subParam[1];
			param.value = subParam[3];

			if (type == "string") {
				param.type = ParameterType::String;
			}
			else if (type == "bool") {
				param.type = ParameterType::Boolean;
			}
			else if (type == "int") {
				param.type = ParameterType::Integer;
			}
			else if (type == "float") {
				param.type = ParameterType::Float;
			}

			paramSet.addParameter(param);
		}

		return paramSet;
	}
};
*/