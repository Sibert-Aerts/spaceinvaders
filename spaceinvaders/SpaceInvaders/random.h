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

			// Get a pointer to the only existing RNG instance
			static std::shared_ptr<RNG> getInstance();

			// Get a random integer between min and max
			template< class IntType = int >
			IntType intFromRange(IntType min, IntType max) {
				std::uniform_int_distribution<IntType> dis(min, max);
				return dis(generator);
			}

			// Get a random real between min and max
			template< class RealType = double>
			RealType realFromRange(RealType min, RealType max) {
				std::uniform_real_distribution<RealType> dis(min,max);
				return dis(generator);
			}

			// Has an x out of y chance of returning 'true'
			bool chanceOutOf(unsigned int x, unsigned int y) {
				return intFromRange(1u, y) <= x;
			}
			
			// Has a 'chance' out of 1 chance of returning 'true'
			bool chanceOutOf(double chance) {
				return realFromRange(0.0, 1.0) <= chance;
			}

		};


	}
}