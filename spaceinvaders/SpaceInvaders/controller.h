#pragma once
#include "StdAfx.h"
#include "Time.h"

#define isPressed(key) sf::Keyboard::isKeyPressed(key)

namespace SI {
	namespace Ctrl {

		// An enum representing user input
		enum Input {
			left, right, shoot, pause, MAX_INPUTNUM = pause
		};
		
		// A class representing a keyboard-based controller
		class Controller {
		private:
			std::bitset<MAX_INPUTNUM + 1> recordedInput;
			Time::BinaryRepeatTimer updateTimer;
			bool read;

		public:
			Controller(double period = 0);

			void update();

			// Returns the registered input as a vector of Input
			// Guaranteed constant return value between update calls
			std::vector<Input> getInput();

		};


	}
}