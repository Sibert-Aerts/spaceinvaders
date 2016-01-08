#include "StdAfx.h"
#include "game.h"

namespace SI {
	
	// Variable determining the minimum amount between 2 ticks in the model
	double modelUpdateInterval = 0.001f;

	Game::Game() : 
		stopwatch(Time::GlobalStopwatch::getInstance()),
		model(modelUpdateInterval)
	{
		controller = std::make_shared<Ctrl::Controller>(0.0f);
		model.registerController(controller);
	}

	void Game::registerView( std::shared_ptr<Vw::View> view ){
		model.registerView(view);
		views.push_back(view);
	}

	void Game::run() {
		model.reset();
		while (true) {
			controller->update();
			model.tick();
			tickViews();
		}
	}

	void Game::tickViews() {
		for (std::shared_ptr<Vw::View> view : views) {
			view->update();
		}
	}

}