#include "StdAfx.h"
#include "tools.h"

namespace SI {

	std::vector<std::string> split(const std::string s, char delim) {
		std::stringstream stream(s);
		std::vector<std::string> out;
		std::string item;
		while (std::getline(stream, item, delim))
			out.push_back(item);

		return out;
	}

	double align(double x, double y) {
		double fmod = std::fmod(x, y);
		if (fmod > y / 2)
			return x - fmod + y;
		return x - fmod;
	}

}