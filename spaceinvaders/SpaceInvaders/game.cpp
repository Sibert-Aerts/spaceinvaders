#include "StdAfx.h"
#include "game.h"

namespace SI {
	
	// Variable determining the minimum time between 2 ticks in the model
	double modelUpdateInterval = 1.0/120.0;

	Game::Game() {
		model = std::unique_ptr<Md::Model>(new Md::Model(modelUpdateInterval));
		controller = std::make_shared<Ctrl::Controller>(0.0);
		model->registerController(controller);
	}

	void Game::registerView( std::shared_ptr<Vw::View> view ){
		model->registerView(view);
		views.push_back(view);
	}

	void Game::run() {
		model->reset();
		while (true) {
			controller->update();
			model->tick();
			updateViews();
		}
	}

	void Game::updateViews() {
		for (std::shared_ptr<Vw::View> view : views) {
			view->update();
		}
	}

}
