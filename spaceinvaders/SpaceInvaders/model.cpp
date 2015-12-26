#include "StdAfx.h"
#include "model.h"


namespace SI {

	namespace Md {


		Model::Model(double tickPeriod) :
			stopwatch(std::make_shared<Time::SimStopwatch>()),
			updateTimer(tickPeriod),
			counter(stopwatch),
			pauseTimer(0.2f),
			gameOver(false),
			playerInvincTimer(3.0f, stopwatch)
		{
			player = std::make_shared<Md::Player>(50, 640, "PLAYER 1", stopwatch);
			payload = std::make_shared<Payload>();
			payload->lives = player->health;
			addEntity(player);
			playerSpawn();
		}

		void Model::registerView(std::shared_ptr<Vw::View> view) {
			view->registerPayload(payload);
		}

		void Model::registerController(std::shared_ptr<Ctrl::Controller> controller) {
			this->controller = controller;
		}

		void Model::tick() {
			
			if (!updateTimer())	// Cancel the tick if it's too soon
				return;
			
				// dt = time since last tick in seconds
			double dt = stopwatch->tick();

				// read inputs based off dt
			tickInput(dt);

			// ____If the game is paused don't do any of the following: ________________
			if (stopwatch->isPaused())
				return;

				// Update the payload on time-related business
			payload->playerInvinc = playerInvincTimer();

				// Separate the Entities
			std::vector<std::shared_ptr<DebugEntity>> debugEntities;
			std::vector<std::shared_ptr<Bullet>> bullets;
			std::vector<std::shared_ptr<Enemy>> enemies;
			std::vector<std::shared_ptr<Barrier>> barriers;
			
			for (auto entity : entities) {
				if (auto& e = std::dynamic_pointer_cast<DebugEntity>(entity))
					debugEntities.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Bullet>(entity))
					bullets.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Enemy>(entity))
					enemies.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Barrier>(entity))
					barriers.push_back(e);
			}
			
				// Tick entities
			for (auto& e : debugEntities)
				tickDebugEntity(dt, e);

			for (auto& e : bullets)
				tickBullet(dt, e, enemies, barriers);

			for (auto& e : enemies)
				tickEnemy(dt, e);

				// Collissions
			if(debugEntities.size() > 1)
				for (unsigned int i = 0; i < debugEntities.size() - 1; ++i)
					for (unsigned int j = i + 1; j < debugEntities.size(); ++j)
						debugEntities[i]->collide(debugEntities[j]);

			payload->secondsPassed = (unsigned int)counter.getSeconds();

		}

		void Model::tickInput(double dt){
			if (gameOver)
				return;
			std::vector<Ctrl::Input> inputs = controller->getInput();
			for (Ctrl::Input input : inputs) {
				switch (input){

				case Ctrl::shoot:
					if (!stopwatch->isPaused())
						if (player->fireCooldown()) {
							addEntity(std::make_shared<Md::PlayerBullet>(player->xpos, player->ypos, 0, -400, 0, -200, 20.0f, 1));
							payload->addEvent(friendlyShotFired);
						}
					break;

				case Ctrl::left:
					if (!stopwatch->isPaused() && player->xpos > 50)
						player->xpos -= dt * player->speed;
					player->updatePosition();
					break;

				case Ctrl::right:
					if (!stopwatch->isPaused() && player->xpos < 750)
						player->xpos += dt * player->speed;
					player->updatePosition();
					break;

				case Ctrl::pause:
					if (pauseTimer()) {
						if (stopwatch->isPaused()) {
							stopwatch->unPause();
							payload->paused = false;
							payload->addEvent(unPaused);
						} else {
							stopwatch->pause();
							payload->paused = true;
							payload->addEvent(paused);
						}
					}
					break;
				}
			}
		}

		void Model::tickDebugEntity(double dt, std::shared_ptr<DebugEntity> e) {
			e->tick(dt);
			if (e->xpos < e->size){
				e->xacc = std::abs(e->xacc);
				e->xvel = std::abs(e->xvel);
			}
			if (e->xpos > 800 - e->size) {
				e->xacc = -std::abs(e->xacc);
				e->xvel = -std::abs(e->xvel);
			}
			if (e->ypos < e->size) {
				e->yacc = std::abs(e->yacc);
				e->yvel = std::abs(e->yvel);
			}
			if (e->ypos > 720 - e->size) {
				e->yacc = -std::abs(e->yacc);
				e->yvel = -std::abs(e->yvel);
			}
				
		}

		void Model::tickBullet(double dt, std::shared_ptr<Bullet> e, std::vector<std::shared_ptr<Enemy>> enemies, std::vector<std::shared_ptr<Barrier>> barriers){
			e->tick(dt);
			for (auto& barrier : barriers) {
				if (e->collide(barrier)) {
					payload->addEvent(Event(barrierHit, e->xpos, e->ypos));
					e->hurt(barrier);
					if (barrier->isDead()) {
						deleteEntity(barrier);
						payload->addEvent(Event(barrierDestroyed, barrier->xpos, barrier->ypos));
					}
				}
			}

			if (auto p = std::dynamic_pointer_cast<PlayerBullet>(e)) {
				for (auto& enemy : enemies) {
					if (p->collide(enemy)) {
						payload->addEvent(Event(enemyHit, enemy->xpos, enemy->ypos));
						p->hurt(enemy);
						if(enemy->isDead())
							deleteEntity(enemy);
					}
				}
			} else if (auto p = std::dynamic_pointer_cast<EnemyBullet>(e)) {
				if (p->collide(player)) {
					deleteEntity(e);
					playerHit();
				}
			}
			if (e->ypos < 0 || e->ypos > 720 || e->isDead())
				deleteEntity(e);
		}
		
		void Model::tickEnemy(double dt, std::shared_ptr<Enemy> e) {
			if (RNG::RNG::getInstance()->chanceOutOf(1, 4000)) {
				addEntity(std::make_shared<Md::EnemyBullet>(e->xpos, e->ypos, RNG::RNG::getInstance()->intFromRange(-20,20), 300, 0, 0));
				payload->addEvent(EventType::enemyShotFired);
			}
		}

		void Model::addEntity(std::shared_ptr<Entity> entity) {
			entities.push_back(entity);
			auto pe = payload->addEntity();
			entity->registerPayloadEntity(pe);
			payload->entityCount = entities.size();
		}

		void Model::deleteEntity(std::shared_ptr<Entity> entity){
			payload->deleteEntity(entity->payloadEntity);
			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
			payload->entityCount = entities.size();
		}

		void Model::playerHit(){
			payload->addEvent(Event(EventType::friendlyHit, player->xpos, player->ypos));
			
			// Don't do anything if the player is invincible
			if (playerInvincTimer())	// shove this timer into the player
				return;

			player->health--;
			payload->lives = player->health;
			if (player->isDead()) {
				stopwatch->pause();
				gameOver = true;
				payload->gameOver = true;
				payload->addEvent(EventType::gameOver);
			}
			else
				playerSpawn();
		}

		void Model::playerSpawn() {
			player->xpos = 400;
			player->updatePosition();
			playerInvincTimer.reset();
		}

		std::vector<std::shared_ptr<Entity>>* Model::getEntities(){
			return &entities;
		}

	};
}