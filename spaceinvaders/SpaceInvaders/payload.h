#pragma once
#include "StdAfx.h"

namespace SI {
	namespace Md {

		// An enum representing a type of event
		enum EventType {
			friendlyShotFired, enemyShotFired, bulletHit, friendlyHit, enemyHit, enemyDestroyed, barrierHit, barrierDestroyed, paused, unPaused, gameOver
		};

		// A struct representing type of event, and possibly its location
		struct Event {
			double x, y;
			EventType type;

			Event(EventType type, double x = 0, double y = 0);
		};

		// An enum representing a crude abstraction of types of entities
		enum EntityType {
			player, enemy, playerBullet, enemyBullet, barrier, powerup
		};


		// A struct representing all the data a view can know about an entity
		struct PayloadEntity {
			EntityType type;
			double xpos, ypos;
			int health;

			PayloadEntity();
		};


		// A struct representing all the data a view can know about a model
		struct Payload {

		// ____VALUES_____________________

			// Variables
			unsigned int secondsPassed;
			unsigned int lives;
			bool gameOver;
			bool playerInvinc;
			bool paused;

			// Debug variables
			unsigned int entityCount;

			// Events, a series of enum values that indicate something happened
			std::vector<Event> events;

			// PayloadEntities, each individually updated by their unique entity
			std::vector<std::shared_ptr<PayloadEntity>> payloadEntities;

		// ____METHODS_____________________

			// Register an event
			void addEvent(Event event);

			// Pop all stored events
			std::vector<Event> popEvents();
			
			// Register and hand over a new entity
			std::shared_ptr<PayloadEntity> addEntity();

			// Remove an entity
			void deleteEntity(std::shared_ptr<PayloadEntity> e);

			Payload();
		};

	}
}