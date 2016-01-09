#include "StdAfx.h"
#include "model.h"


namespace SI {

	namespace Md {
		Model::Model(double tickPeriod) :
			stopwatch(std::make_shared<Time::SimStopwatch>()),
			updateTimer(tickPeriod),
			counter(stopwatch),
			pauseTimer(0.2f),
			playerInvincTimer(3.0, true, stopwatch),
			playerDeadTimer(2.0, true, stopwatch),
			levelSwitchTimer(3.0, true),
			currentLevel(0)
		{
			levelParser = std::make_unique<LevelParser>();
			enemyCluster = std::make_unique<EnemyCluster>(stopwatch);

			levels = levelParser->parseLevels();
		}

		void Model::reset()	{
			levelSwitchTimer.forceFalse();
			playerDeadTimer.forceFalse();
			playerInvincTimer.forceFalse();
			updateState(ModelState::running);

			player = std::make_shared<Md::Player>(50, 640, stopwatch);
			updateLives(3);

			currentLevel = 0;
			loadLevel();
		}

		void Model::registerView(std::shared_ptr<Vw::View> view) {
			observers.push_back(view->getObserver());
		}

		void Model::registerController(std::shared_ptr<Ctrl::Controller> controller) {
			this->controller = controller;
		}

		void Model::clearEntities(){
			entities.clear();
			for(auto& observer : observers)
				observer->clearEntities();
		}

		void Model::updateState(ModelState state){
			this->state = state;
			for (auto& observer : observers)
				observer->updateState(state);
		}

		void Model::updateLives(int lives){
			player->setHealth(lives);
			for (auto& observer : observers)
				observer->updateLives(lives);
		}

		void Model::updatePlayerState(){
			for (auto& observer : observers) {
				observer->updatePlayerDead(playerDeadTimer());
				observer->updatePlayerInvinc(playerInvincTimer());
			}
		}

		void Model::updateLevelName(){
			for (auto& observer : observers)
				observer->updateLevelName(levels[currentLevel]->getName());
		}

		void Model::updateSecondsPassed(){
			for (auto& observer : observers)
				observer->updateSecondsPassed((unsigned int)counter.getSeconds());
		}
		
		void Model::retryLevel(){
			stopwatch->unPause();
			updateLives(3);
			updateState(ModelState::running);
			loadLevel();
		}

		void Model::loadLevel(){

			clearEntities();
			enemyCluster->clear();
			enemyCluster->setSpeed(levels[currentLevel]->getSpeed(), levels[currentLevel]->getSpeedInc());

			updateLevelName();
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
			updateState(ModelState::levelSwitch);
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
			if(state != ModelState::levelSwitch)
				tickInput(dt);

				// If we're in the LevelComplete state, check if we can leave the state, otherwise don't do anything
			if (state == ModelState::levelSwitch && !levelSwitchTimer()) {
				updateState(ModelState::running);
				stopwatch->unPause();
			}

			if (stopwatch->isPaused())
				return;

			// ____ THE FOLLOWING ONLY HAPPENS IF THE GAME IS UNPAUSED: ____________________

				// Update the payload on time-related business
			updatePlayerState();

				// Separate the Entities
			std::vector<std::shared_ptr<Bullet>> bullets;
			std::vector<std::shared_ptr<Enemy>> enemies;
			std::vector<std::shared_ptr<Barrier>> barriers;
			std::vector<std::shared_ptr<Powerup>> powerups;
			
			for (auto& entity : entities) {
				if (auto& e = std::dynamic_pointer_cast<Bullet>(entity))
					bullets.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Enemy>(entity))
					enemies.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Barrier>(entity))
					barriers.push_back(e);
				if (auto& e = std::dynamic_pointer_cast<Powerup>(entity))
					powerups.push_back(e);
			}
			
				// Tick all entities appropriately
			
			for (auto& e : bullets)
				tickBullet(dt, e, enemies, barriers);

			for (auto& e : enemies)
				tickEnemy(dt, e, barriers);

			for (auto& e : powerups)
				tickPowerup(dt, e);

			// Tick the enemy cluster
			enemyCluster->tick(dt);

			// if the enemies reach the bottom of the screen, the game is over
			if (enemyCluster->lowestPoint() > 720)
				gameOver();

			if (enemyCluster->count() == 0)
				completeLevel();
			
			updateSecondsPassed();

		}

		void Model::tickInput(double dt){
			std::vector<Ctrl::Input> inputs = controller->getInput();
			for (Ctrl::Input input : inputs) {
				switch (input){

				case Ctrl::shoot:
					if (state == ModelState::running && !playerDeadTimer())
						player->shoot();
					//if (state == ModelState::paused && !levelSwitchTimer()) {
					//	completeLevel();
					//	std::cout << "SKIPPING LEVEL" << std::endl;
					//}
					break;

				case Ctrl::left:
					if (state == ModelState::running && !playerDeadTimer())
						player->moveLeft(dt);
					break;

				case Ctrl::right:
					if (state == ModelState::running && !playerDeadTimer())
						player->moveRight(dt);
					break;

				case Ctrl::pause:
					if (pauseTimer()) {	// Rid me of this filthy thing, and this whole damn switch case!!!!!!!	
						if (state == ModelState::paused) {					// If paused, escape unpauses
							stopwatch->unPause();
							updateState(ModelState::running);
							addEvent(Event(unPaused));
						} else if (state == ModelState::running){										// If unpaused, escape pauses
							stopwatch->pause();
							updateState(ModelState::paused);
							addEvent(Event(paused));
						}
					} else if (state == ModelState::gameOver) {			// If game over, escape retries the level
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
					addEvent(Event(bulletHit, e->getX(), e->getY()));
					deleteEntity(e);
					playerHit();
				}
			}
			if (e->getX() < 0 || e->getY() > 720 || e->isDead())
				deleteEntity(e);
		}
		
		void Model::tickEnemy(double dt, std::shared_ptr<Enemy> e, std::vector<std::shared_ptr<Barrier>> barriers) {
			e->tick(dt);
			for (auto& barrier : barriers) {
				if (e->hit(barrier)) {
					e->hurt(barrier);
					if (e->isDead()) {
						deleteEntity(e);
						e->destroyEvent();
					}
				}
			}
		}

		void Model::tickPowerup(double dt, std::shared_ptr<Powerup> e){
			e->tick(dt);
			if (e->hit(player)) {
				deleteEntity(e);
				switch (e->getPowerupType()) {
				case speedUp:
					player->speedUp();
					addEvent(Event(pickup, e->getX(), e->getY(), "SPEED UP!"));
					break;
				case bulletSpeedUp:
					player->bulletSpeedUp();
					addEvent(Event(pickup, e->getX(), e->getY(), "SHOT UP!"));
					break;
				case fireRateUp:
					player->fireRateUp();
					addEvent(Event(pickup, e->getX(), e->getY(), "FIRE UP!"));
					break;
				case damageUp:
					player->damageUp();
					addEvent(Event(pickup, e->getX(), e->getY(), "DMG UP!"));
					break;
				case slowdown:
					enemyCluster->freeze();
					addEvent(Event(pickup, e->getX(), e->getY(), "FREEZE!"));
					break;
				}
			}
			// Destroy the powerup if it falls off the bottom of the screen
			if (e->getY() > 720.0)
				deleteEntity(e);
		}

		void Model::addEvent(Event& e){
			for(auto& observer : observers)
				observer->addEvent(e);
		}

		void Model::addEntity(std::shared_ptr<Entity> entity) {

			entity->registerModel(this);
			entities.push_back(entity);
			if (auto &e = std::dynamic_pointer_cast<Enemy>(entity))
				enemyCluster->addEnemy(e);

			for (auto& observer : observers) {
				auto pe = observer->addEntity();
				entity->registerObserver(pe);
				observer->updateEntityCount(entities.size());
			}
		}

		void Model::deleteEntity(std::shared_ptr<Entity> entity){

			entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
			if (auto &e = std::dynamic_pointer_cast<Enemy>(entity))
				enemyCluster->deleteEnemy(e);

			for (unsigned int i = 0; i < observers.size(); ++i) {
				observers[i]->deleteEntity(entity->getObservers()[i]);
				observers[i]->updateEntityCount(entities.size());
			}
		}

		void Model::playerHit(){
			addEvent(Event(EventType::friendlyHit, player->getX(), player->getY()));
			
			// Don't do anything if the player is invincible
			if (playerInvincTimer())
				return;

			updateLives(player->getHealth() - 1);
			if (player->isDead()) {
				gameOver();
			}
			else
				playerSpawn();
		}

		void Model::playerSpawn() {
			player->setX(400);
			player->resetPowerups();
			player->updatePosition();
			playerDeadTimer.reset();
			playerInvincTimer.reset();
		}

		void Model::gameOver() {
			stopwatch->pause();
			updateState(ModelState::gameOver);
			addEvent(Event(EventType::gameOver));
		}

	};
}