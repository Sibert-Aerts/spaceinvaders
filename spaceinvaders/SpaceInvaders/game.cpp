#include "StdAfx.h"
#include "game.h"

namespace SI {

	// VARIABLES (TODO: READ THESE FROM AN INI OR SOMETHING)
	
	double modelUpdateInterval = 0.001f;

	Game::Game() : 
		stopwatch(Time::GlobalStopwatch::getInstance()),
		model(modelUpdateInterval)
	{
		controller = std::make_shared<Ctrl::Controller>(0.0f);
		model.registerController(controller);
		
		// TODO: read this out of an xml or something
		for (int i = 0; i < 6; ++i)
			for (int j = 0; j < 3; ++j)
				model.addEntity(std::make_shared<Md::Enemy>(100 + 120*i, 80+80*j));


		for (int i = 0; i < 6; ++i) {
			model.addEntity(std::make_shared<Md::Barrier>(80 + 120 * i, 560));
			model.addEntity(std::make_shared<Md::Barrier>(120 + 120 * i, 560));
		}

	}

	void Game::registerView( std::shared_ptr<Vw::View> view ){
		model.registerView(view);
		views.push_back(view);
	}

	void Game::run() {
		while (true) {
			controller->update();
			model.tick();
			tickViews();
			std::this_thread::sleep_for(std::chrono::milliseconds(2));
		}
	}

	void Game::tickViews() {
		for (std::shared_ptr<Vw::View> view : views) {
			view->update();
		}
	}

}