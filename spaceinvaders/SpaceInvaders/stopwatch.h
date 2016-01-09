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

		// An abstract class that allows entities to keep track of time passed between ticks
		// and the current time, according to the stopwatch's rules
		class Stopwatch {
		protected:
			// Data members
			TimePoint lastTick;

		public:
			Stopwatch();
			virtual double tick() = 0;
			virtual TimePoint now() const = 0;

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

			// Get the amount of time since last tick() call
			double tick();

			// Get the current time point in real time
			TimePoint now() const;
		};


		// A Stopwatch designed for a simulation to use,
		// Can have multiple instances, can be paused
		class SimStopwatch : public Stopwatch {
		private:
			std::shared_ptr<GlobalStopwatch> globalStopwatch;

			// Whether the stopwatch is paused or not
			bool paused;

			// The moment the stopwatch was paused
			TimePoint pauseTime;

			// The amount of time the stopwatch has been paused
			std::chrono::nanoseconds pauseAdjust;

		public:
			SimStopwatch();

			// Get the amount of time since last tick() call, adjusted by the model's pausedness
			double tick();

			// Get the current time point, adjusted by the model's pausedness
			TimePoint now() const;

			// Get whether or not the stopwatch is paused
			bool isPaused() const;

			// Pause the stopwatch
			void pause();

			// Unpause the stopwatch
			void unPause();

		};

	}
}