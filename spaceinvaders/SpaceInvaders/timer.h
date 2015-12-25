#pragma once
#include "stopwatch.h"

namespace SI {
	namespace Time {

		// An interface for objects which return 'true' when called if certain conditions
		// regarding a given period, a past point in time and the current point in time
		// are met, working based off the time given by a certain Stopwatch.
		class PeriodTimer {
		protected:
			std::shared_ptr<Stopwatch> stopwatch;
			TimePoint timePoint;
			unsigned long long period;

		public:
			PeriodTimer(double period, std::shared_ptr<Stopwatch> stopwatch = GlobalStopwatch::getInstance());

			virtual void setPeriod(double period);

			// Equivalent to making a new PeriodTimer with the same period
			virtual void reset();

			// Check if the time is right
			virtual bool operator()() = 0;

			virtual double timePassed();
		};

		// A timer which returns 'true' when called if it has been longer than 'period' s since the last 
		// time it returned 'true'.
		// ex: last returned 'true' 10 seconds ago with period 2: will return 'true'
		//
		// For when a certain action needs to happen at most once every 'period'
		class BinaryRepeatTimer : public PeriodTimer {
		public:
			BinaryRepeatTimer(double period, std::shared_ptr<Stopwatch> = GlobalStopwatch::getInstance());
			
			// Returns 'true' if it has been longer than 'period' since the last time it returned 'true'
			virtual bool operator()();
		};

		// A timer which returns 'true' for as many times as 'period' seconds can be put into the time between
		// its creation and the current time
		// ex: created 10 seconds ago with period 2: will return 'true' 5 times if repeatedly called
		//
		// For when a certain action needs to happen regularly, and must be caught up on if missed
		class CountingRepeatTimer : public PeriodTimer {
		protected:
			unsigned int periods;
		public:
			CountingRepeatTimer(double period, std::shared_ptr<Stopwatch> = GlobalStopwatch::getInstance());

			virtual void setPeriod(double period);
			virtual void reset();

			// Returns true if another 'period' fits in the time since its creation
			virtual bool operator()();

			// Takes up all the periods that still fit in the time since its creation, returns their count.
			virtual unsigned int count();
		};

		// A timer that checks if it's been less than 'period' time since its creation or the last reset
		class WithinPeriodTimer : public PeriodTimer {
		public:

			WithinPeriodTimer(double period, std::shared_ptr<Stopwatch> stopwatch = GlobalStopwatch::getInstance());

			virtual bool operator()();
		};
	}
}