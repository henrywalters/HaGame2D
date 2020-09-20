#pragma once
#include "Color.h"

struct Style {
	double width = 150;
	double height = 30;
	
	RGB foregroundColor = Color::black();
	RGB backgroundColor = Color::green();
	RGB borderColor = Color::white();

	int fontSize = 16;
};