#include "StdAfx.h"
#include "payload.h"

namespace SI {
	namespace Md {

		// Event

		Event::Event(EventType type, double x, double y) :
			type(type), x(x), y(y) {}
		
		// Payload

		void Payload::addEvent(Event event) {
			events.push_back(event);
		}

		std::vector<Event> Payload::popEvents() {
			std::vector<Event> out = events;
			events.clear();
			return out;
		}

		std::shared_ptr<PayloadEntity> Payload::addEntity() {
			auto newEntity = std::make_shared<PayloadEntity>();
			payloadEntities.push_back(newEntity);
			return newEntity;
		}

		void Payload::deleteEntity(std::shared_ptr<PayloadEntity> e) {
			payloadEntities.erase(std::remove(payloadEntities.begin(), payloadEntities.end(), e), payloadEntities.end());
		}

		Payload::Payload() :
			secondsPassed(0),
			paused(false),
			gameOver(false),
			levelComplete(false),
			playerDead(false)
		{}
		
		PayloadEntity::PayloadEntity() {}
	}
}