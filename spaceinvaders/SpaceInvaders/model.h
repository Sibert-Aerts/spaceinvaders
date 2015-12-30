#pragma once

#include "StdAfx.h"
	// Model stuff
#include "level.h"
#include "payload.h"
#include "entity.h"
	// Other
#include "view.h"
#include "controller.h"
#include "time.h"
#include "random.h"

namespace SI {

	namespace Vw {
		class View;
	}

	namespace Md {

		class Entity;
		class Player;
		class Enemy;
		class EnemyCluster;
		class Barrier;
		class Bullet;
		class FriendlyBullet;
		class EnemyBullet;
		class Powerup;

		class LevelParser;
		class Level;
		
	// A model capable of simulating a game of Space Invaders
		class Model {

		private:
			// Data members
			Time::BinaryRepeatTimer updateTimer;

			std::shared_ptr<Ctrl::Controller> controller;
			std::shared_ptr<Time::SimStopwatch> stopwatch;

			std::unique_ptr<LevelParser> levelParser;
			unsigned int currentLevel;
			std::vector<std::shared_ptr<Level>> levels;

			std::vector<std::shared_ptr<Entity>> entities;
			std::unique_ptr<EnemyCluster> enemyCluster;

			std::shared_ptr<Player> player;
			Time::WithinPeriodTimer playerInvincTimer;	// shove this timer into the player?
			Time::WithinPeriodTimer playerDeadTimer;	// shove this timer into the player?

			ModelState state;
			Time::WithinPeriodTimer levelSwitchTimer;
			Time::BinaryRepeatTimer pauseTimer;	// get rid of this sick filth

			// The model's own payload
			std::shared_ptr<Payload> payload;

			// The counter keeping track of how long the level has gone on for
			Time::Counter counter;

		public:
			Model(double tickPeriod = 0);

			// Register a view
			void registerView(std::shared_ptr<Vw::View> view);

			// Register the controller
			void registerController(std::shared_ptr<Ctrl::Controller> controller);

			// Remove all entities
			void clearEntities();

			// Retry the current level
			void retryLevel();

			// Load the current level
			void loadLevel();

			// Complete the current level and move on to the next, or win the game
			void completeLevel();

			// Advance the simulation by a single step
			// But only if it has been long enough since the last tick
			void tick();

			// Read and act according to the given inputs
			void tickInput(double dt);

			// Advance a bullet by a single step
			void tickBullet(double dt, std::shared_ptr<Bullet> e, std::vector<std::shared_ptr<Enemy>> enemies, std::vector<std::shared_ptr<Barrier>> barriers);

			void tickEnemy(double dt, std::shared_ptr<Enemy> e, std::vector<std::shared_ptr<Barrier>> barriers);

			// Register a new entity to the simulation
			void addEvent(Event& e);

			// Register a new entity to the simulation
			void addEntity(std::shared_ptr<Entity> entity);

			// Delete an entity from the simulation
			void deleteEntity(std::shared_ptr<Entity> entity);

			// Hit the player
			void playerHit();

			// (re-)spawn the player
			void playerSpawn();

			// "game-over" the player
			void gameOver();

		};
	}
}