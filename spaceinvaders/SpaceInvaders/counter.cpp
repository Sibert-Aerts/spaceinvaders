#include "StdAfx.h"
#include "counter.h"

namespace SI {
	namespace Time {

		Counter::Counter(std::shared_ptr<Stopwatch> sw) : 
			stopwatch(sw)
		{
			start = stopwatch->now();
		}

		void Counter::reset() {
			start = stopwatch->now();
		}

		double Counter::getSeconds() const {
			return nanoToSeconds(stopwatch->now() - start);
		}

		// PeriodCounter

		PeriodCounter::PeriodCounter(double period, std::shared_ptr<Stopwatch> sw) : 
			counter(sw), period(period)
		{}

		unsigned int PeriodCounter::getCount() const {
			return (unsigned int)(counter.getSeconds() / period);
		}


	}
}