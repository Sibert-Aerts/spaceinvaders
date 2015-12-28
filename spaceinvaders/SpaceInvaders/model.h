#pragma once

#include "StdAfx.h"
#include "view.h"
#include "controller.h"
#include "time.h"
#include "entity.h"
#include "payload.h"
#include "random.h"

namespace SI {

	namespace Vw {
		class View;
	}

	namespace Md {

	// A model capable of simulating a game of Space Invaders
		class Model {

		private:
			// Data members
			std::shared_ptr<Ctrl::Controller> controller;
			std::shared_ptr<Time::SimStopwatch> stopwatch;

			std::vector<std::shared_ptr<Entity>> entities;
			EnemyCluster enemyCluster;

			std::shared_ptr<Player> player;
			Time::WithinPeriodTimer playerInvincTimer;	// shove this timer into the player

			bool gameOverState;
			Time::BinaryRepeatTimer updateTimer;
			Time::BinaryRepeatTimer pauseTimer;

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

			// Advance the simulation by a single step
			// But only if it has been long enough since the last tick
			void tick();

			// Read and act according to the given inputs
			void tickInput(double dt);

			// Advance a DebugEntity by a single step
			void tickDebugEntity(double dt, std::shared_ptr<DebugEntity> e);
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