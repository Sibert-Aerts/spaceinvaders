#include "StdAfx.h"
#include "model.h"


namespace SI {

	namespace Md {


		Model::Model(double tickPeriod) :
			stopwatch(std::make_shared<Time::SimStopwatch>()),
			updateTimer(tickPeriod, stopwatch),
			counter(stopwatch),
			pauseTimer(0.5f)
		{
			player = std::make_shared<Md::Player>(50, 500, "toeterman");
			addEntity(player);
		}

		void Model::registerView(std::shared_ptr<Vw::View> view) {
			views.push_back(view);
		}

		void Model::registerController(std::shared_ptr<Ctrl::Controller> controller) {
			this->controller = controller;
		}

		void Model::tick() {
			
			tickPausedInput();

			if (!updateTimer())	// Cancel the tick if it's too soon
				return;
			
				// dt = time since last tick in seconds
			double dt = stopwatch->tick();

				// read inputs based off dt
			tickInput(dt);

				// Separate the Entities 
			std::vector<std::shared_ptr<DebugEntity>> debugEntities;
			std::vector<std::shared_ptr<Bullet>> bullets;
			std::vector<std::shared_ptr<Enemy>> enemies;
			
			for (auto entity : entities) {
				if (auto& e = std::dynamic_pointer_cast<DebugEntity>(entity))
					debugEntities.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Bullet>(entity))
					bullets.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Enemy>(entity))
					enemies.push_back(e);
			}

				// Tick entities
			for (auto& e : debugEntities)
				tickDebugEntity(dt, e);

			for (auto& e : bullets)
				tickBullet(dt, e, enemies);

				// Collissions
			for (unsigned int i = 0; i < debugEntities.size() - 1; ++i) {
				for (unsigned int j = i + 1; j < debugEntities.size(); ++j)
					debugEntities[i]->collide(debugEntities[j]);
			}

		}

		void Model::tickInput(double dt){
			std::vector<Ctrl::Input> inputs = controller->getInput();
			for (Ctrl::Input input : inputs) {
				switch (input){

				case Ctrl::shoot:
					if(player->fireCooldown())
						addEntity(std::make_shared<Md::Bullet>(player->xpos, player->ypos, 0, -400));
					break;

				case Ctrl::left:
					player->xpos -= dt * 200;
					break;

				case Ctrl::right:
					player->xpos += dt * 200;
					break;

				case Ctrl::pause:
					// TODO: Implement pause or somethin'
					if (pauseTimer())
						stopwatch->pause();
					break;
				}
			}
		}

		void Model::tickPausedInput() {
			if (!stopwatch->isPaused())
				return;

			std::vector<Ctrl::Input> inputs = controller->getInput();
			for (Ctrl::Input input : inputs) {
				switch (input) {

				case Ctrl::shoot:
					break;

				case Ctrl::left:
					break;

				case Ctrl::right:
					break;

				case Ctrl::pause:
					if(pauseTimer())
						stopwatch->unPause();
					return;
				}
			}
		}

		void Model::tickDebugEntity(double dt, std::shared_ptr<DebugEntity> e) {
			e->tick(dt);
			if (e->xpos < e->size){
				e->xacc = std::abs(e->xacc);
				e->xvel = std::abs(e->xvel);
			}
			if (e->xpos > 800- e->size) {
				e->xacc = -std::abs(e->xacc);
				e->xvel = -std::abs(e->xvel);
			}
			if (e->ypos < e->size) {
				e->yacc = std::abs(e->yacc);
				e->yvel = std::abs(e->yvel);
			}
			if (e->ypos > 600- e->size) {
				e->yacc = -std::abs(e->yacc);
				e->yvel = -std::abs(e->yvel);
			}
				
		}

		void Model::tickBullet(double dt, std::shared_ptr<Bullet> e, std::vector<std::shared_ptr<Enemy>> enemies){
			e->tick(dt);
			for (auto& enemy : enemies) {
				if (e->collide(enemy)) {
					deleteEntity(e);
					deleteEntity(enemy);
					return;
				}
			}
			if (e->ypos < 0)
				deleteEntity(e);
		}

		void Model::updateViews(double dt) {
			for (std::shared_ptr<Vw::View> view : views) {
				view->update(entities);
			}
		}

		void Model::addEntity(std::shared_ptr<Entity> entity) {
			entities.push_back(entity);
		}

		void Model::deleteEntity(std::shared_ptr<Entity> entity){
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
		}

		std::vector<std::shared_ptr<Entity>>& Model::getEntities(){
			return entities;
		}

	};
}