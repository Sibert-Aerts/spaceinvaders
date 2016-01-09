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

			// Check which keys are pressed
			void update();

			// Returns the registered input as a vector of Input
			// Will return the exact same vector until update() is called again
			std::vector<Input> getInput();

		};


	}
}