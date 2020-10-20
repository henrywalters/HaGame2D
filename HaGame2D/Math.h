#pragma once
#include "Vector.h"

class Math
{
public:
	Math();
	~Math();

	static double clamp(double val, double min, double max) {
		if (val > max) return max;
		else if (val < min) return min;
		else return val;
	}

	static int sign(double numb) {
		return numb >= 0 ? 1 : -1;
	}

	//Returns the spacing required to satisfy the condition that a set of subElements are evenly distributed in a larger element.
	static double getUniformDistrubtionSpacing(double totalSize, double subSize, int subElementCount) {
		return (totalSize - subSize * subElementCount) / (subElementCount + 1);
	}

	//Maps the index of a set of subElements to the value of the lesser side of where the subElement belongs in the larger element. 
	static double mapToUniformDistribution(double subSize, double spacing, int subElementIndex) {
		return subElementIndex * (subSize + spacing) + spacing;
	}

	static Vector rotateVector(Vector vect, double theta) {
		return rotatePointAroundPoint(vect, Vector::Zero(), theta);
	}

	static Vector rotatePointAroundPoint(Vector point, Vector origin, double theta) {
		double s = sin(theta);
		double c = cos(theta);

		Vector vect;

		vect.x = c * (point.x - origin.x) - s * (point.y - origin.y) + origin.x;
		vect.y = s * (point.x - origin.x) + c * (point.y - origin.y) + origin.y;
		vect.z = point.z;

		return vect;
	}

};

