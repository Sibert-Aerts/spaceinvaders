#include "StdAfx.h"
#include "random.h"

namespace SI {
	namespace RNG {

	// Class: RNG

		// private:
			// Static data member:
		std::shared_ptr<RNG> RNG::self;

		RNG::RNG() : 
			generator(randomDevice())
		{}


		// public:
		
		std::shared_ptr<RNG> RNG::getInstance() {
			if (self == NULL)
				self = std::shared_ptr<RNG>(new RNG);
			return self;
		}
	}
}