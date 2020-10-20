#pragma once
#include "Vector.h"
#include "Line.h"
#include <vector>
#include <iostream>
#include <optional>
#include <functional>

#ifndef H_ALGORITHMS
#define H_ALGORITHMS

class Algorithms {
public:

	template <class T>
	static T Max(T arr[], int n) {
		T max = 0;
		bool first = false;
		for (int i = 0; i < n; i++) {
			if (!first) {
				max = arr[i];
				first = true;
			} else if (arr[i] > max) {
				max = arr[i];
			}
		}
		return max;
	}

	template <class T>
	static T Min(T arr[], int n) {
		T min = 0;
		bool first = false;
		for (int i = 0; i < n; i++) {
			if (!first) {
				min = arr[i];
				first = true;
			}
			else if (min == NULL || arr[i] < min) {
				min = arr[i];
			}
		}
		return min;
	}

	template <class I, class O>
	static int MinIndex(std::vector<I> arr, std::function<O(I)> fn) {
		O m = 0;
		int mIndex = 0;
		bool first = false;
		for (int i = 0; i < arr.size(); i++) {
			O f = fn(arr[i]);
			if (!first) {
				m = f;
				first = true;
			}
			else if (f < m) {
				m = f;
				mIndex = i;
			}
		}
		return mIndex;
	}

	/**
	*	Returns the clipped line of a axis-aligned box and line collision.
	*	Repurposed from https://en.wikipedia.org/wiki/Liang%E2%80%93Barsky_algorithm
	*/
	static std::optional<Line> LiangBarsky(float xmin, float ymin, float xmax, float ymax, float x1, float y1, float x2, float y2) {
		float p1 = -(x2 - x1);
		float p2 = -p1;
		float p3 = -(y2 - y1);
		float p4 = -p3;

		float q1 = x1 - xmin;
		float q2 = xmax - x1;
		float q3 = y1 - ymin;
		float q4 = ymax - y1;

		float pArr[5], nArr[5];
		int pInd = 1, nInd = 1;

		pArr[0] = 1;
		nArr[0] = 0;

		if ((p1 == 0 && q1 < 0) || (p2 == 0 && q2 < 0) || (p3 == 0 && q3 < 0) || (p4 == 0 && q4 < 0)) {
			// The line is parallel to the box
			return std::nullopt;
		}

		if (p1 != 0) {
			float r1 = q1 / p1;
			float r2 = q2 / p2;

			if (p1 < 0) {
				nArr[nInd++] = r1;
				pArr[pInd++] = r2;
			} else {
				nArr[nInd++] = r2;
				pArr[pInd++] = r1;
			}
		}

		if (p3 != 0) {
			float r3 = q3 / p3;
			float r4 = q4 / p4;

			if (p3 < 0) {
				nArr[nInd++] = r3;
				pArr[pInd++] = r4;
			} else {
				nArr[nInd++] = r4;
				pArr[pInd++] = r3;
			}
		}

		float xn1, yn1, xn2, yn2;
		float rn1, rn2;

		rn1 = Algorithms::Max<float>(nArr, nInd);
		rn2 = Algorithms::Min<float>(pArr, pInd);

		if (rn1 > rn2) {
			// Line is outside of clipping window
			return std::nullopt;
		}

		xn1 = x1 + p2 * rn1;
		yn1 = y1 + p4 * rn1;

		xn2 = x1 + p2 * rn2;
		yn2 = y1 + p4 * rn2;

		return Line(Vector(xn1, yn1), Vector(xn2, yn2));
	}
};

#endif