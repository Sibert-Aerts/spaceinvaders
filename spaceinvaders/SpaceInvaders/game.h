#pragma once

#include "model.h"
#include "controller.h"
#include "view.h"
#include "time.h"
#include "random.h"


namespace SI {
	
	// A simple struct that holds game configuration data.
	struct GameConfig {
		int foo;
		int bar;
	};

	// A class which manages a Model, Controllers and Views into
	// a functioning program.
	class Game {
	private:
			// Data members
		Md::Model model;
		std::vector<std::shared_ptr<Vw::View>> views;
		std::shared_ptr<Ctrl::Controller> controller;

		std::shared_ptr<Time::GlobalStopwatch> stopwatch;

		unsigned int tickNum;

	public:
		Game();

		void registerView( std::shared_ptr<Vw::View> view );

		void run();

		void tickViews();

	};






}