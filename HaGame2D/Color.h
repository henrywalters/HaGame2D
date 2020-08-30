#pragma once

struct RGB {
	int r;
	int g;
	int b;
	int a;
};

class Color
{
public:

	RGB rgb;

	Color();

	Color(int r, int g, int b) {
		Color(r, g, b, 0xFF);
	}

	Color(int r, int g, int b, int a) {
		rgb.r = r;
		rgb.g = g;
		rgb.b = b;
		rgb.a = a;
	}

	Color(char * hex) {
		rgb = parseHex(hex);
	}

	~Color();

	static RGB red() {
		return RGB { 0xFF, 0x00, 0x00, 0xFF };
	}

	static RGB green() {
		return RGB { 0x00, 0xFF, 0x00, 0xFF };
	}

	static RGB blue() {
		return RGB { 0x00, 0x00, 0xFF, 0xFF };
	}

	static RGB white() {
		return RGB { 0xFF, 0xFF, 0xFF, 0xFF };
	}

	static RGB black() {
		return RGB{ 0x00, 0x00, 0x00, 0xFF };
	}

	static RGB parseHex(char * hex) {
		int r, g, b;
		sscanf_s(hex, "#%02x%02x%02x", &r, &g, &b);
		return RGB { r, g, b, 0xFF };
	}
};

