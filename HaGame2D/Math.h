#pragma once
class Math
{
public:
	Math();
	~Math();

	static int sign(float numb) {
		return numb >= 0 ? 1 : -1;
	}

};

