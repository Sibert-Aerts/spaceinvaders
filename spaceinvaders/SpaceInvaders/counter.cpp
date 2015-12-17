#include "StdAfx.h"
#include "counter.h"

namespace SI {
	namespace Time {

		Counter::Counter(std::shared_ptr<Stopwatch> sw) {
			if (sw)
				stopwatch = sw;
			else
				stopwatch = GlobalStopwatch::getInstance();
			start = stopwatch->now();
		}

		void Counter::reset() {
			start = stopwatch->now();
		}

		double Counter::getSeconds() {
			return nanoToSeconds(stopwatch->now() - start);
		}
	}
}