#pragma once
#include <random>
#include <time.h>
#include <chrono>

class Random
{
	std::mt19937 generator;
public:
	Random();
	Random(int seed);
	~Random();

	int generate(int lower, int upper);
	static int number(int lower, int upper);
};

