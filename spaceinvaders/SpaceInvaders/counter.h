#pragma once

#include "stopwatch.h"

namespace SI {
	namespace Time {

		// A simple class that counts the seconds that have passed since a moment, by a certain stopwatch
		class Counter {
		protected:
			TimePoint start;
			std::shared_ptr<Stopwatch> stopwatch;

		public:
			Counter(std::shared_ptr<Stopwatch> sw = GlobalStopwatch::getInstance());

			void reset();

			double getSeconds();

		};

		// A class that counts how many times a certain period has passed since its creation, useful for timed animations
		class PeriodCounter{
		protected:
			Counter counter;
			double period;

		public:
			PeriodCounter(double period, std::shared_ptr<Stopwatch> sw = GlobalStopwatch::getInstance());

			unsigned int getCount();

		};

	}
}