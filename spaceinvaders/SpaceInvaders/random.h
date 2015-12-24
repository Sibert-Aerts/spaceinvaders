#pragma once

#include "StdAfx.h"

namespace SI {
	namespace RNG {

	// A Singleton class that offers various Random Number Generating tools
		class RNG {
		private:
			// Self pointer
			static std::shared_ptr<RNG> self;

			// RNG entities
			std::random_device randomDevice;
			std::mt19937 generator;

			RNG();

		public:

			static std::shared_ptr<RNG> getInstance();

			template< class IntType = int >
			IntType intFromRange(IntType min, IntType max) {
				std::uniform_int_distribution<IntType> dis(min, max);
				return dis(generator);
			}

			bool chanceOutOf(unsigned int x, unsigned int y) {
				return intFromRange(1u, y) <= x;
			}

		};


	}
}