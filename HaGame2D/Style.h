#pragma once
#include "Color.h"

struct Style {
	float width = 150;
	float height = 30;
	
	RGB foregroundColor = Color::black();
	RGB backgroundColor = Color::green();
	RGB borderColor = Color::white();

	int fontSize = 16;
};