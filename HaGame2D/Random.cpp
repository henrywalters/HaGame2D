#include "stdafx.h"
#include "Random.h"


Random::Random()
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	generator.seed(seed);
}

Random::Random(int seed)
{
	generator.seed(seed);
}



Random::~Random()
{
}

int Random::generate(int lower, int upper)
{
	return generator() % (upper - lower) + lower;
}

int Random::number(int lower, int upper)
{
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);

	return generator() % (upper - lower) + lower;
}