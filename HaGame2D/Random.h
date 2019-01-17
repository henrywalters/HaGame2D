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

	//returns [lower, upper)
	static int number(int lower, int upper);

	//returns (-upper, -lower] union [lower, upper)
	static int numberPlusMinus(int lower, int upper);
};

