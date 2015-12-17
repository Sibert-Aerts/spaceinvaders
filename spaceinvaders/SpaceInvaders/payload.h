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

			// Updated by the pause functionality
			bool paused;

			// PayloadEntities, each individually updated by their unique entity
			std::vector<PayloadEntity> entities;

		};

	}
}