#include "StdAfx.h"
#include "stopwatch.h"

namespace SI
{
	namespace Time {

	// Helper Functions

		double nanoToSeconds(std::chrono::nanoseconds nano) {
			return (double)(nano.count()) / 1e9f;
		}

	// Stopwatch

		Stopwatch::Stopwatch() : lastTick(std::chrono::seconds(0)) {}

	// GlobalStopwatch : public Stopwatch

		// private:	
			// static data member:
		std::shared_ptr<GlobalStopwatch> GlobalStopwatch::stopwatch;

		GlobalStopwatch::GlobalStopwatch() : Stopwatch(){}

		// public:
		std::shared_ptr<GlobalStopwatch> GlobalStopwatch::getInstance() {
			if (!stopwatch) {
				stopwatch = std::shared_ptr<GlobalStopwatch>(new GlobalStopwatch);
			}
			return stopwatch;
		}

		TimePoint GlobalStopwatch::now() const {
			return std::chrono::high_resolution_clock::now();
		}

		double GlobalStopwatch::tick() {
			TimePoint now = this->now();

			if (!lastTick.time_since_epoch().count()) {
				lastTick = now;
				return 0;
			}
			double out = nanoToSeconds(now - lastTick);
			lastTick = now;
			return out;
		}

	// SimStopwatch

		SimStopwatch::SimStopwatch() : globalStopwatch(GlobalStopwatch::getInstance()), paused(false), pauseAdjust(0) {}

		TimePoint SimStopwatch::now() const {
			if (paused)
				return (pauseTime - pauseAdjust);
			return (globalStopwatch->now() - pauseAdjust);
		}

		double SimStopwatch::tick() {
			// No tick length while paused
			if (paused)
				return 0;

			TimePoint now = this->now();

			if (!lastTick.time_since_epoch().count()) {
				lastTick = now;
				return 0;
			}
			double out = nanoToSeconds(now - lastTick);
			lastTick = now;
			return out;			
		}

		bool SimStopwatch::isPaused() const {
			return paused;
		}

		void SimStopwatch::pause() {
			// Don't execute if already paused
			if (paused)
				return;

			paused = true;
			pauseTime = globalStopwatch->now();
		}

		void SimStopwatch::unPause() {
			// Don't execute if not paused
			if (!paused)
				return;

			paused = false;
			pauseAdjust += (globalStopwatch->now() - pauseTime);
		}
	}
}