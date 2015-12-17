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

		// Debug
		model.addEntity(std::make_shared<Md::DebugEntity>(20, 20, 8, 8, 5, 7, 20.0f));
		model.addEntity(std::make_shared<Md::DebugEntity>(20, 200, 5, 10, 3, -1, 5.0f));
		model.addEntity(std::make_shared<Md::DebugEntity>(200, 20, 5, -10, 10, -0, 10.0f));

		model.addEntity(std::make_shared<Md::DebugEntity>(400, 400, 5, -10, -20, -2, 15.0f));
		model.addEntity(std::make_shared<Md::DebugEntity>(300, 100, 4, -3, -3, -1, 30.0f));
		model.addEntity(std::make_shared<Md::DebugEntity>(100, 300, 4, 3, 7, -5, 15.0f));

		for (int i = 0; i < 14; ++i)
			for (int j = 0; j < 4; ++j)
				model.addEntity(std::make_shared<Md::Enemy>(60+50*i, 60+50*j));

	}

	void Game::registerView( std::shared_ptr<Vw::View> view ){
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
			view->update(model.getEntities());
		}
	}

}