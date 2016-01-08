#pragma once
#include "StdAfx.h"

namespace SI {
	namespace Md {

		class Model;

		// An enum representing the states the model can be in
		enum class ModelState {
			running, paused, gameOver, levelSwitch
		};

		// An enum representing a type of event
		enum EventType {
			friendlyShotFired, enemyShotFired, bulletHit, friendlyHit, enemyHit, smallEnemyDestroyed, bigEnemyDestroyed, 
			barrierHit, barrierDestroyed, pickup, 
			paused, unPaused, gameOver
		};

		// A struct representing type of event, and possibly its location
		struct Event {
		protected:
			double x, y;
			EventType type;
			std::string text;
		public:
			Event(EventType type, double x = 0, double y = 0, std::string text = "");
			double getX();
			double getY();
			EventType getType();
			std::string getText();
		};
				
		// An enum representing a crude abstraction of types of entities
		enum EntityType {
			player, smallEnemy, bigEnemy, playerBullet, enemyBullet, barrier, powerup
		};

		// A struct representing all the data a view can know about an entity
		class EntityObserver {
		private:
			EntityType type;
			double xpos, ypos;
			int health;

		public:
			EntityObserver();

			EntityType getType();
			void setType(EntityType type);
			double getXpos();
			void setPos(double xpos, double ypos);
			double getYpos();
			int getHealth();
			void setHealth(int health);

		};


		// A struct representing all the data a view can know about a model
		class ModelObserver {

		// ____VALUES_____________________
					
			// Variables
			std::string levelName;
			unsigned int secondsPassed;
			int lives;
			ModelState state;
			bool playerInvinc;
			bool playerDead;
			unsigned int entityCount;	// Debug

			// Events, a series of enum values that indicate something happened
			std::vector<Event> events;

			// PayloadEntities, each individually updated by their unique entity
			std::vector<std::shared_ptr<EntityObserver>> entityObservers;

		public:
			std::string getLevelName();
			void updateLevelName(std::string levelName);
			unsigned int getSecondsPassed();
			void updateSecondsPassed(unsigned int secondsPassed);
			int getLives();
			void updateLives (int lives);
			ModelState getState();
			void updateState (ModelState state);
			bool isPlayerInvinc();
			void updatePlayerInvinc(bool playerInvinc);
			bool isPlayerDead();
			void updatePlayerDead(bool playerDead);
			unsigned int getEntityCount();
			void updateEntityCount(unsigned int entityCount);


			std::vector<std::shared_ptr<EntityObserver>>& getEntityObservers();

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

			ModelObserver();
		};

	}
}