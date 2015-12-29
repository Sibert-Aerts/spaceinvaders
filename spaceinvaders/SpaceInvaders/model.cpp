#include "StdAfx.h"
#include "model.h"


namespace SI {

	namespace Md {

		Model::Model(double tickPeriod) :
			stopwatch(std::make_shared<Time::SimStopwatch>()),
			updateTimer(tickPeriod),
			counter(stopwatch),
			pauseTimer(0.2f),
			gameOverState(false),
			playerInvincTimer(3.0, true, stopwatch),
			playerDeadTimer(2.0, true, stopwatch),
			levelSwitchTimer(3.0, true), levelComplete(false),
			currentLevel(0)
		{
			// Initialise the static Entity variable pointing to the model:
			Entity::model = this;

			player = std::make_shared<Md::Player>(50, 640, "PLAYER 1", stopwatch);
			payload = std::make_shared<Payload>();

			levels = levelParser.parseLevels();

			loadLevel();
		}

		void Model::registerView(std::shared_ptr<Vw::View> view) {
			view->registerPayload(payload);
		}

		void Model::registerController(std::shared_ptr<Ctrl::Controller> controller) {
			this->controller = controller;
		}

		void Model::clearEntities(){
			entities.clear();
			payload->payloadEntities.clear();
		}

		void Model::retryLevel(){
			stopwatch->unPause();
			player->health = 3;
			gameOverState = payload->gameOver = false;
			loadLevel();
		}

		void Model::loadLevel(){

			clearEntities();
			enemyCluster.clear();

			payload->lives = player->health;
			addEntity(player);
			playerSpawn();
			playerDeadTimer.forceFalse();
			playerInvincTimer.forceFalse();

			auto newEntities = levels[currentLevel]->makeEntities();
			for (auto& e : newEntities)
				addEntity(e);
		}

		void Model::completeLevel(){
			if (currentLevel == levels.size() - 1) {
				gameOver();
				return;
			}
			currentLevel++;
			levelComplete = payload->levelComplete = true;
			stopwatch->pause();
			levelSwitchTimer.reset();
			loadLevel();
		}

		void Model::tick() {
			
			if (!updateTimer())	// Cancel the tick if it's too soon
				return;
			
				// dt = time since last tick in seconds
			double dt = stopwatch->tick();

				// read inputs based off dt
			tickInput(dt);

			if (levelComplete && !levelSwitchTimer()) {
				levelComplete = payload->levelComplete = false;
				stopwatch->unPause();
			}

			if (stopwatch->isPaused())
				return;

			// ____ THE FOLLOWING ONLY HAPPENS IF THE GAME IS UNPAUSED: ____________________

				// Update the payload on time-related business
			payload->playerDead = playerDeadTimer();
			payload->playerInvinc = playerInvincTimer();

				// Separate the Entities
			std::vector<std::shared_ptr<DebugEntity>> debugEntities;
			std::vector<std::shared_ptr<Bullet>> bullets;
			std::vector<std::shared_ptr<Enemy>> enemies;
			std::vector<std::shared_ptr<Barrier>> barriers;
			
			for (auto& entity : entities) {
				if (auto& e = std::dynamic_pointer_cast<DebugEntity>(entity))
					debugEntities.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Bullet>(entity))
					bullets.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Enemy>(entity))
					enemies.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Barrier>(entity))
					barriers.push_back(e);
			}
			
				// Tick all entities appropriately

			for (auto& e : debugEntities)
				e->tick(dt);

			for (auto& e : bullets)
				tickBullet(dt, e, enemies, barriers);

			for (auto& e : enemies)
				tickEnemy(dt, e, barriers);

			// Tick the enemy cluster
			enemyCluster.tick(dt);

			// if the enemies reach the bottom of the screen, the game is over
			if (enemyCluster.lowestPoint() > 720)
				gameOver();

			if (enemyCluster.count() == 0)
				completeLevel();

				// Collissions
			if(debugEntities.size() > 1)
				for (unsigned int i = 0; i < debugEntities.size() - 1; ++i)
					for (unsigned int j = i + 1; j < debugEntities.size(); ++j)
						debugEntities[i]->collide(debugEntities[j]);

			payload->secondsPassed = (unsigned int)counter.getSeconds();

		}

		void Model::tickInput(double dt){
			std::vector<Ctrl::Input> inputs = controller->getInput();
			for (Ctrl::Input input : inputs) {
				switch (input){

				case Ctrl::shoot:
					if (!stopwatch->isPaused() && !playerDeadTimer())
						player->shoot();
					break;

				case Ctrl::left:
					if (!stopwatch->isPaused() && !playerDeadTimer())
						player->moveLeft(dt);
					break;

				case Ctrl::right:
					if (!stopwatch->isPaused() && !playerDeadTimer())
						player->moveRight(dt);
					break;

				case Ctrl::pause:
					if (pauseTimer() && !gameOverState) {
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
					else if (gameOverState) {
						retryLevel();
					}
					break;
				}
			}
		}

		void Model::tickBullet(double dt, std::shared_ptr<Bullet> e, std::vector<std::shared_ptr<Enemy>> enemies, std::vector<std::shared_ptr<Barrier>> barriers){
			e->tick(dt);
			for (auto& barrier : barriers)
				if (e->hit(barrier))
					e->hurt(barrier);

			if (auto p = std::dynamic_pointer_cast<PlayerBullet>(e)) {
				for (auto& enemy : enemies)
					if (p->hit(enemy))
						p->hurt(enemy);
				
			} else if (auto p = std::dynamic_pointer_cast<EnemyBullet>(e)) {
				if (p->hit(player) && !playerDeadTimer()) {
					payload->addEvent(Event(bulletHit, e->xpos, e->ypos));
					deleteEntity(e);
					playerHit();
				}
			}
			if (e->ypos < 0 || e->ypos > 720 || e->isDead())
				deleteEntity(e);
		}
		
		void Model::tickEnemy(double dt, std::shared_ptr<Enemy> e, std::vector<std::shared_ptr<Barrier>> barriers) {
			e->tick(dt);
			for (auto& barrier : barriers) {
				if (e->hit(barrier)) {
					e->hurt(barrier);
					if (e->isDead()) {
						deleteEntity(e);
						addEvent(Event(enemyDestroyed, e->xpos, e->ypos));
					}
				}
			}
		}

		void Model::addEvent(Event& e){
			payload->addEvent(e);
		}

		void Model::addEntity(std::shared_ptr<Entity> entity) {

			entities.push_back(entity);
			if (auto &e = std::dynamic_pointer_cast<Enemy>(entity))
				enemyCluster.addEnemy(e);

			auto pe = payload->addEntity();
			entity->registerPayloadEntity(pe);

			payload->entityCount = entities.size();
		}

		void Model::deleteEntity(std::shared_ptr<Entity> entity){

			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
			if (auto &e = std::dynamic_pointer_cast<Enemy>(entity))
				enemyCluster.deleteEnemy(e);

			payload->deleteEntity(entity->payloadEntity);

			payload->entityCount = entities.size();
		}

		void Model::playerHit(){
			payload->addEvent(Event(EventType::friendlyHit, player->xpos, player->ypos));
			
			// Don't do anything if the player is invincible
			if (playerInvincTimer())
				return;

			player->health--;
			payload->lives = player->health;
			if (player->isDead()) {
				gameOver();
			}
			else
				playerSpawn();
		}

		void Model::playerSpawn() {
			player->xpos = 400;
			player->updatePosition();
			playerDeadTimer.reset();
			playerInvincTimer.reset();
		}

		void Model::gameOver() {
			stopwatch->pause();
			gameOverState = true;
			payload->gameOver = true;
			payload->addEvent(EventType::gameOver);
		}

	};
}