#include "StdAfx.h"
#include "game.h"

namespace SI {

	// VARIABLES (TODO: READ THESE FROM AN INI OR SOMETHING)
	
	double modelUpdateInterval = 0.015f;

	Game::Game() : 
		stopwatch(Time::GlobalStopwatch::getInstance()),
		model(modelUpdateInterval)
	{
		controller = std::make_shared<Ctrl::Controller>(0.0f);
		model.registerController(controller);
		
		for (int i = 0; i < 6; ++i)
			for (int j = 0; j < 4; ++j)
				model.addEntity(std::make_shared<Md::Enemy>(80+120*i, 60+80*j));

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