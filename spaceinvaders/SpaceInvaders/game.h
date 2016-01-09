#pragma once

#include "model.h"
#include "controller.h"
#include "view.h"
#include "time.h"
#include "random.h"


namespace SI {

	// A class which manages a Model, Controllers and Views into a working game
	class Game {
	private:
			// Data members
		// The model
		std::shared_ptr<Md::Model> model;

		// The views
		std::vector<std::shared_ptr<Vw::View>> views;
		
		// The controller
		std::shared_ptr<Ctrl::Controller> controller;

	public:
		Game();

		// Register a view
		void registerView( std::shared_ptr<Vw::View> view );

		// Begin running the game, endlessly
		void run();

		// Update all views
		void updateViews();

	};






}