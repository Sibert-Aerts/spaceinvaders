#pragma once

#include "StdAfx.h"
#include "level.h"
#include "observer.h"
#include "entity.h"
#include "view.h"
#include "controller.h"
#include "time.h"
#include "random.h"

namespace SI {

	// Declare classes to resolve dependencies
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
		
		// A model which simulates a game of Space Invaders
		class Model {

		private:
				// Various:
			// A timer that determines the minimum time between ticks
			Time::BinaryRepeatTimer updateTimer;

			// The controller
			std::shared_ptr<Ctrl::Controller> controller;
			
			// The stopwatch used as a basis for the simulation, can be paused
			std::shared_ptr<Time::SimStopwatch> stopwatch;
			
				// Level related:
			// Parses levels
			std::unique_ptr<LevelParser> levelParser;

			// Index of the current level
			unsigned int currentLevel;

			// Set of parsed levels
			std::vector<std::shared_ptr<Level>> levels;

			// The counter keeping track of how long the level has gone on for
			Time::Counter counter;

				// Entity related:
			// The entities handled by the model
			std::vector<std::shared_ptr<Entity>> entities;

			// A cluster of enemies
			std::unique_ptr<EnemyCluster> enemyCluster;

				// Player related:
			// An additional pointer to the player entity
			std::shared_ptr<Player> player;

			// A timer that determines if the player is invincible
			Time::WithinPeriodTimer playerInvincTimer;

			// A timer that determines if the player is dead
			Time::WithinPeriodTimer playerDeadTimer;

				// State related:
			// The current state of the player/model
			ModelState state;

			// A timer that determines if the level is switching
			Time::WithinPeriodTimer levelSwitchTimer;

			// A timer that determines if the game can be paused/unpaused (necessary because of lack of keyDown events)
			Time::BinaryRepeatTimer pauseTimer;

			// The model's observers
			std::vector<std::shared_ptr<ModelObserver>> observers;

		public:
			Model(double tickPeriod = 0);

			// Reset the model to its most basic state
			// Has to be called at least once before the model can be used
			void reset();

			// Register a view
			void registerView(std::shared_ptr<Vw::View> view);

			// Update the observers on specific state changes
			void updateState(ModelState state);
			void updateLives(int lives);
			void updatePlayerState();
			void updateLevelName();
			void updateSecondsPassed();

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

			// Set the game in the victory state
			void victory();

			// Advance the simulation by a single step
			// But only if it has been long enough since the last tick
			void tick();

			// Read and act according to the given inputs
			void tickInput(double dt);

			// Advance a bullet by a single step and check collissions
			void tickBullet(double dt, std::shared_ptr<Bullet> e, std::vector<std::shared_ptr<Enemy>> enemies, std::vector<std::shared_ptr<Barrier>> barriers);

			// Advance an enemy by a single step and check collissions
			void tickEnemy(double dt, std::shared_ptr<Enemy> e, std::vector<std::shared_ptr<Barrier>> barriers);

			// Advance a powerup by a single step and check collissions
			void tickPowerup(double dt, std::shared_ptr<Powerup> e);

			// Register a new entity to the simulation
			void addEvent(const Event& e);

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
