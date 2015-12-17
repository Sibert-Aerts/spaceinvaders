#pragma once

#include "StdAfx.h"

typedef std::chrono::time_point<std::chrono::high_resolution_clock> TimePoint;

namespace SI
{
	namespace Time {

	// Helper functions

		// Returns the # of seconds from a std::chrono::nanoseconds
		double nanoToSeconds(std::chrono::nanoseconds nano);
		

	// Classes

		// An abstract class that allows entities to keep track of time passed between ticks,
		// and the current time, according to the stopwatch's rules
		class Stopwatch {
		protected:
			// Data members
			TimePoint lastTick;

		public:
			Stopwatch();
			virtual double tick() = 0;
			virtual TimePoint now() = 0;

		};

		class GlobalStopwatch;

		// A stopwatch based off the computer's internal clock
		// Can only be instantiated once, cannot be paused or altered
		class GlobalStopwatch : public Stopwatch {
		private:
			// Data members
			static std::shared_ptr<GlobalStopwatch> stopwatch;

			// Functions
			GlobalStopwatch();

		public:
			static std::shared_ptr<GlobalStopwatch> getInstance();

			double tick();
			TimePoint now();
		};


		// A Stopwatch designed for a simulation to use,
		// Can have multiple instances, can be paused
		class SimStopwatch : public Stopwatch {
		private:
			std::shared_ptr<GlobalStopwatch> globalStopwatch;
			TimePoint lastTick;

			bool paused;
			TimePoint pauseTime;
			std::chrono::nanoseconds pauseAdjust;

		public:
			SimStopwatch();

			double tick();
			TimePoint now();
			bool isPaused();
			void pause();
			void unPause();

		};

	}
}