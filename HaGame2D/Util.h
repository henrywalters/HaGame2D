#pragma once
#include <string>
#include <vector>
#include <functional>

class Util
{
public:
	// Print a bunch of items in a cpp vector.
	template<class T>
	static std::string toString(std::vector<T> vect, std::function<std::string(T)> toStringFn, std::string delim = "");
};

template<class T>
inline std::string Util::toString(std::vector<T> vect, std::function<std::string(T)> toStringFn, std::string delim)
{
	std::string out = "";
	int vLen = vect.size();

	if (vLen >= 1) {
		for (int i = 0; i < vLen; i++) {
			out += toStringFn(vect[i]);
			if (i < vLen - 1) {
				out += delim;
			}
		}
	}

	return out;
}