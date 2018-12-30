#pragma once
#include <string>

class Loggable
{
public:
	Loggable();
	~Loggable();

	virtual std::string toString() {
		return "";
	};
};

