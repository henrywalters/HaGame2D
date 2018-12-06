#pragma once

struct RGB {
	int r;
	int g;
	int b;
};

class Color
{
public:

	RGB rgb;

	Color();
	Color(int r, int g, int b) {
		rgb.r = r;
		rgb.g = g;
		rgb.b = b;
	}

	Color(char * hex) {
		rgb = parseHex(hex);
	}

	~Color();

	static RGB red() {
		return RGB { 0xFF, 0x00, 0x00 };
	}

	static RGB green() {
		return RGB { 0x00, 0xFF, 0x00 };
	}

	static RGB blue() {
		return RGB { 0x00, 0x00, 0xFF };
	}

	static RGB white() {
		return RGB { 0xFF, 0xFF, 0xFF };
	}

	static RGB black() {
		return RGB{ 0x00, 0x00, 0x00 };
	}

	static RGB parseHex(char * hex) {
		int r, g, b;
		sscanf_s(hex, "#%02x%02x%02x", &r, &g, &b);
		return RGB { r, g, b };
	}
};

