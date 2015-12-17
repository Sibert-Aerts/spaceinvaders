#pragma once

#include "StdAfx.h"
#include "view.h"
#include "controller.h"
#include "time.h"
#include "entity.h"
#include "random.h"

namespace SI {

	namespace Vw {
		class View;
	}

	namespace Md {

	// A model capable of simulating a game of Space Invaders
		class Model
			: public std::enable_shared_from_this<Model> {

		private:
			// Data members
			std::shared_ptr<Ctrl::Controller> controller;
			std::vector<std::shared_ptr<Vw::View>> views;
			std::shared_ptr<Time::SimStopwatch> stopwatch;

			std::vector<std::shared_ptr<Entity>> entities;
			std::shared_ptr<Player> player;
			Time::BinaryRepeatTimer updateTimer;
			Time::BinaryRepeatTimer pauseTimer;


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
		// Read and react to inputs-- while paused !
			void tickPausedInput();

		// Advance a DebugEntity by a single step
			void tickDebugEntity(double dt, std::shared_ptr<DebugEntity> e);
		// Advance a bullet by a single step
			void tickBullet(double dt, std::shared_ptr<Bullet> e, std::vector<std::shared_ptr<Enemy>> enemies);

		// Inform the views of what the haps are
		// TODO: Shove this further down in the entitites, each of which should update their own mirror entity
		// or give some type of payload or something, who knows.
			void updateViews(double dt);

		// Register a new entity to the simulation
			void addEntity(std::shared_ptr<Entity> entity);

		// Delete an entity from the simulation
			void deleteEntity(std::shared_ptr<Entity> entity);

		// Get the entities like the greedy fucking View you are
		// You can't do that! You can't touch that! The Model has to give these to you!
		// TODO: Make obsolete
			std::vector<std::shared_ptr<Entity>>& getEntities();

		};

	}

}