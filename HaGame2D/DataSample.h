#pragma once
#include <string>
#include <iostream>
const int DEFAULT_SAMPLE_SIZE = 100;

template <class T>
class DataSample
{
	int sampleSize;

	int index = 0;

	T sum = 0;

	std::string tag;
public:
	DataSample(std::string _tag = "Sample", int _sampleSize = DEFAULT_SAMPLE_SIZE) {
		sampleSize = _sampleSize;
		tag = _tag;
	}

	~DataSample() {

	}

	void add(T sample) {
		if (index % sampleSize == 0) {
			float avg = sum / sampleSize;

			std::cout << tag << " average: " << avg << "\n";

			sum = sample;
			index = 1;
		}
		else {
			sum += sample;
			index += 1;
		}
	}
};

