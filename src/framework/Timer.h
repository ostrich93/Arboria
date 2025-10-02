#ifndef TIMER_H
#define TIMER_H

#include "../globals.h"

namespace Arboria {
	class Timer {
		private:
			double _timepoint = getMicroseconds();
		public:
			void restart() noexcept {
				_timepoint = getMicroseconds();
			}

			//get elapsed time in microseconds
			double getElapsedTime() const noexcept {
				return getMicroseconds() - _timepoint;
			}

			double getElapsedTimeAndRestart() noexcept {
				const auto res = getMicroseconds() - _timepoint;
				restart();
				return res;
			}
	};
}

#endif