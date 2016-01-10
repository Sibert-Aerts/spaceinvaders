#pragma once
#include "StdAfx.h"

namespace SI {
	namespace Md {

		class Model;

		// An enum representing the states the model can be in
		enum class ModelState {
			running, paused, gameOver, levelSwitch, victory
		};

		// An enum representing a type of event
		enum EventType {
			friendlyShotFired, bulletHit, friendlyHit, pickup,
			enemyHit, enemyShotFired, smallEnemyDestroyed, bigEnemyDestroyed,
			barrierHit, barrierDestroyed, 
			paused, unPaused, gameOver
		};

		// A struct representing type of event, and possibly its location and related text
		struct Event {
		protected:
			// The optional x and y values of the event
			double x, y;

			// The optional text of the event
			std::string text;

			// The type of the event
			EventType type;
		public:
			Event(EventType type, double x = 0, double y = 0, std::string text = "");

			// Get the x value
			double getX();

			// Get the y value
			double getY();

			// Get the text value
			std::string getText();

			// Get the event's type
			EventType getType();
		};
				
		// An enum representing a crude abstraction of types of entities
		enum EntityType {
			player, smallEnemy, bigEnemy, playerBullet, enemyBullet, barrier, powerup
		};

		// A struct representing all the data a view can know about an entity
		class EntityObserver {
		private:
			// The type of Entity that's being observed
			EntityType type;

			// The last observed x and y positions of the observed Entity
			double xpos, ypos;

			// The last observed health value of the observed Entity
			int health;

		public:
			EntityObserver();

			// Get the type of the observed entity
			EntityType getType() const;

			// As an entity, update your observed type
			void updateType(EntityType type);

			// Get the xpos of the of the observed entity
			double getXpos() const;
			// Get the ypos of the of the observed entity
			double getYpos() const;

			// As an entity, update your observed position
			void updatePosition(double xpos, double ypos);

			// Get the health value of the of the observed entity
			int getHealth() const;

			// As an entity, update your observed health value
			void updateHealth(int health);

		};


		// A struct representing all the data a view can know about a model
		class ModelObserver {

		// ____VALUES_____________________
					
			// The observed name of the current level
			std::string levelName;

			// The observed number of seconds that have passed since the start of the game
			unsigned int secondsPassed;

			// The observed number of lives the player has
			int lives;

			// The observed state of the model
			ModelState state;

			// The observed states of the player
			bool playerInvinc;
			bool playerDead;

			// The observed number of entities, for debug purposes
			unsigned int entityCount;

			// A number of observed Events
			std::vector<Event> events;

			// A number of PayloadEntities, each uniquely updated by their own specific Entity
			std::vector<std::shared_ptr<EntityObserver>> entityObservers;

		public:
			ModelObserver();

			// Get and update the observed level name
			std::string getLevelName() const;
			void updateLevelName(std::string levelName);

			// Get and update the observed seconds passed
			unsigned int getSecondsPassed() const;
			void updateSecondsPassed(unsigned int secondsPassed);

			// Get and update the observed player lives
			int getLives() const;
			void updateLives (int lives);

			// Get and update the observed model state
			ModelState getState() const;
			void updateState (ModelState state);

			// Get and update the observed player states
			bool isPlayerInvinc() const;
			void updatePlayerInvinc(bool playerInvinc);
			bool isPlayerDead() const;
			void updatePlayerDead(bool playerDead);

			// Get and update the observed entity count, for debug purposes
			unsigned int getEntityCount();
			void updateEntityCount(unsigned int entityCount);

			// Get a reference to the vector of EntityObservers, so that they may be drawn
			const std::vector<std::shared_ptr<EntityObserver>>& getEntityObservers() const;

			// Register an event
			void addEvent(Event event);

			// Pop all stored events
			std::vector<Event> popEvents();
			
			// Start observing a new entity and return a pointer to its observer
			std::shared_ptr<EntityObserver> addEntity();

			// Remove an entity
			void deleteEntity(std::shared_ptr<EntityObserver> e);

			// Clear all entities
			void clearEntities();
		};

	}
}
