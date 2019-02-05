#pragma once
class Math
{
public:
	Math();
	~Math();

	static int sign(float numb) {
		return numb >= 0 ? 1 : -1;
	}

	//Returns the spacing required to satisfy the condition that a set of subElements are evenly distributed in a larger element.
	static float getUniformDistrubtionSpacing(float totalSize, float subSize, int subElementCount) {
		return (totalSize - subSize * subElementCount) / (subElementCount + 1);
	}

	//Maps the index of a set of subElements to the value of the lesser side of where the subElement belongs in the larger element. 
	static float mapToUniformDistribution(float subSize, float spacing, int subElementIndex) {
		return subElementIndex * (subSize + spacing) + spacing;
	}

};

