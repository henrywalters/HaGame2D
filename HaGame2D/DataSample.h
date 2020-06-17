#pragma once
#include <string>
#include <iostream>
#include <functional>

const int DEFAULT_SAMPLE_SIZE = 100;

template <class T>
class DataSample
{
	int sampleSize;

	int index = 0;

	T sum = 0;

	std::string tag;

	bool hasCallback = false;
	std::function<void(T)> callbackFn;
public:
	DataSample(std::string _tag = "Sample", int _sampleSize = DEFAULT_SAMPLE_SIZE) {
		sampleSize = _sampleSize;
		tag = _tag;
	}

	~DataSample() {

	}

	void onFullSample(std::function<void(T)> callback) {
		callbackFn = callback;
		hasCallback = true;
	}

	void add(T sample) {
		if (index % sampleSize == 0) {
			float avg = sum / sampleSize;

			if (hasCallback) {
				callbackFn(avg);
			}
			else {
				std::cout << tag << " average: " << avg << "\n";
			}

			sum = sample;
			index = 1;
		}
		else {
			sum += sample;
			index += 1;
		}
	}
};

