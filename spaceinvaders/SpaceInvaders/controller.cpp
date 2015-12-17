#include "StdAfx.h"
#include "controller.h"

namespace SI {
	namespace Ctrl {


	// Controller:
		Controller::Controller(double period) :
			updateTimer(period, Time::GlobalStopwatch::getInstance()),
			recordedInput(0),
			read(false)
		{}

		void Controller::update() {
			if (!updateTimer())
				return;

			if (read) {	// If the the input has been read since last update, clear all recorded input
				recordedInput.reset();
				read = false;
			}

			if (isPressed(sf::Keyboard::Left)  || isPressed(sf::Keyboard::W) || isPressed(sf::Keyboard::Q)) {
				recordedInput[left] = true;
			}
			if (isPressed(sf::Keyboard::Right) || isPressed(sf::Keyboard::D)) {
				recordedInput[right] = true;
			}
			if (isPressed(sf::Keyboard::Space) || isPressed(sf::Keyboard::Z) || isPressed(sf::Keyboard::W)) {
				recordedInput[shoot] = true;
			}
			if (isPressed(sf::Keyboard::Escape)) {
				recordedInput[pause] = true;
			}
		}

		std::vector<Input> Controller::getInput() {
			
			std::vector<Input> out;

			for (unsigned int i = 0; i < MAX_INPUTNUM + 1; ++i) {
				if (recordedInput[i]) {
					out.push_back(Input(i));
				}
			}
			read = true;
			return out;
		}
	
	}
}

