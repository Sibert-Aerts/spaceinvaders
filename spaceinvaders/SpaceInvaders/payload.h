#pragma once
#include "StdAfx.h"
#include "model.h"

namespace SI {
	namespace Md {

		// A struct representing all the data a view can know about an entity
		struct PayloadEntity {

		};

		// A struct representing all the data a view can know about a model
		struct Payload {

			// Updated by the internal clock
			unsigned int secondsPassed;
			unsigned int lives;
			bool gameOver;

			// Updated by the pause functionality
			bool paused;

			// PayloadEntities, each individually updated by their unique entity
			//std::vector<PayloadEntity> entities;

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