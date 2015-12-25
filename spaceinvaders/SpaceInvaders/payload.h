#pragma once
#include "StdAfx.h"
#include "model.h"

namespace SI {
	namespace Md {

		// An enum representing a type of event
		enum EventType {
			friendlyShotFired, enemyShotFired, friendlyHit, enemyHit, barrierHit, barrierDestroyed, paused, unPaused, gameOver
		};

		// A struct representing type of event, and possibly its location
		struct Event {
			double x, y;
			EventType type;

			Event(EventType type, double x = 0, double y = 0) :
				type(type), x(x), y(y) {}
		};

		// An enum representing a crude abstraction of types of entities
		enum EntityType {
			player, enemy, playerBullet, enemyBullet, barrier, powerup
		};


		// A struct representing all the data a view can know about an entity
		struct PayloadEntity {
			EntityType type;
			double xpos, ypos;
			unsigned int health;

			PayloadEntity(){}
		};


		// A struct representing all the data a view can know about a model
		struct Payload {

			// Updated by the internal clock
			unsigned int secondsPassed;
			unsigned int lives;
			bool gameOver;
			bool playerInvinc;

			// Updated by the pause functionality
			bool paused;

			// Events, a series of enum values that indicate something happened
			std::vector<Event> events;

			void addEvent(Event event) {
				events.push_back(event);
			}

			std::vector<Event> popEvents() {
				std::vector<Event> out = events;
				events.clear();
				return out;
			}

			// PayloadEntities, each individually updated by their unique entity
			std::vector<std::shared_ptr<PayloadEntity>> payloadEntities;

			// Register and hand over a new entity
			std::shared_ptr<PayloadEntity> addEntity() {
				auto newEntity = std::make_shared<PayloadEntity>();
				payloadEntities.push_back(newEntity);
				return newEntity;
			}

			// Remove an entity
			void deleteEntity(std::shared_ptr<PayloadEntity> e ) {
				payloadEntities.erase(std::remove(payloadEntities.begin(), payloadEntities.end(), e), payloadEntities.end());
			}

			// Early: a pointer to the entity vector
			std::vector<std::shared_ptr<Entity>>* entities;

			Payload(std::vector<std::shared_ptr<Entity>>* entities) :
				secondsPassed(0),
				paused(false),
				gameOver(false),
				entities(entities)
			{}
		};

	}
}