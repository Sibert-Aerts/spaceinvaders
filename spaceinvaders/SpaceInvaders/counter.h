#pragma once

#include "stopwatch.h"

namespace SI {
	namespace Time {

		// A simple class that counts the seconds that have passed since a moment, by a certain stopwatch
		class Counter {
		private:
			TimePoint start;
			std::shared_ptr<Stopwatch> stopwatch;

		public:
			Counter(std::shared_ptr<Stopwatch> sw = NULL);

			void reset();

			double getSeconds();

		};

	}
}