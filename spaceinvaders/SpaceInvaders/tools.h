#pragma once

#include "StdAfx.h"

// A header containing helpful objects that aren't necessarily part of one specific part of the game

namespace SI {
	
	// An object that's passed values and will return the average value of the last complete period of 'len' inputs
	// Useful for FPS readouts, perhaps other things
	template <typename T, unsigned int len>
	struct AverageAccumulator{
		T elements[len];
		T average;
		unsigned int index;

		AverageAccumulator() {
			index = 0;
		}

		T operator()(T in) {
			elements[index] = in;
			index++;
			if (index == len) {
				average = std::accumulate(elements, elements + len, T())/len;
				index = 0;
			}
			return average;
		}
	};


	// A function that splits a line over a delimiter
	std::vector<std::string> split(const std::string s, char delim);


	// Round a value x to the closest multiple of y
	// Used for aligning to the fake pixel grid
	double align(double x, double y);

}