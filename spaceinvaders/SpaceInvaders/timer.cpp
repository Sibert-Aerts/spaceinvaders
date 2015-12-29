#include "StdAfx.h"
#include "timer.h"

namespace SI {
	namespace Time {

		// PeriodTimer

		PeriodTimer::PeriodTimer(double period, std::shared_ptr<Stopwatch> stopwatch) :
			period((unsigned long long)(period*1e9f)),
			timePoint(std::chrono::high_resolution_clock::now()),
			stopwatch(stopwatch) {}

		void PeriodTimer::setPeriod(double period){
			this->period = (unsigned long long)(period * 1e9f);
		}

		void PeriodTimer::reset(){
			timePoint = stopwatch->now();
		}

		double PeriodTimer::timePassed() {
			return nanoToSeconds(stopwatch->now() - timePoint);
		}

		// BinaryRepeatTimer : PeriodTimer

		BinaryRepeatTimer::BinaryRepeatTimer(double period, std::shared_ptr<Stopwatch> stopwatch) :
			PeriodTimer(period, stopwatch) {}

		bool BinaryRepeatTimer::operator()() {
			auto now = stopwatch->now();
			if ((unsigned long long)(now - timePoint).count() < period) {
				return false;
			}
			else {
				timePoint = now;
				return true;
			}
		}
		
		// CountingRepeatTimer : PeriodTimer

		CountingRepeatTimer::CountingRepeatTimer(double period, std::shared_ptr<Stopwatch> stopwatch) :
			PeriodTimer(period, stopwatch),
			periods(0) {}

		void CountingRepeatTimer::setPeriod(double period) {
			timePoint = TimePoint(std::chrono::nanoseconds(timePoint.time_since_epoch().count() + periods*this->period));
			this->period = (unsigned long long)(period * 1e9f);
			periods = 0;
		}

		void CountingRepeatTimer::reset() {
			timePoint = stopwatch->now();
			periods = 0;
		}

		bool CountingRepeatTimer::operator()() {
			auto now = stopwatch->now();
			if ((now - timePoint).count() - periods*period < period)
				return false;

			periods++;
			return true;
		}

		unsigned int CountingRepeatTimer::count(){
			// Calculate the additional ones that might fit in the open time
			auto diff = (((stopwatch->now() - timePoint).count()) / period) - periods;
			periods += (unsigned int)diff;
			return (unsigned int)diff;
		}

		// WithinPeriodTimer : PeriodTimer

		WithinPeriodTimer::WithinPeriodTimer(double period, bool forceFalseState, std::shared_ptr<Stopwatch> stopwatch) :
			PeriodTimer(period, stopwatch), forceFalseState(forceFalseState) {}

		void WithinPeriodTimer::forceFalse(){
			forceFalseState = true;
		}

		void WithinPeriodTimer::reset(){
			forceFalseState = false;
			PeriodTimer::reset();
		}

		bool WithinPeriodTimer::operator()(){
			return !forceFalseState && ((stopwatch->now() - timePoint).count() < period);
		}
	}
}