#pragma once
#include "StdAfx.h"
#include "time.h"

namespace SI {
	namespace Vw {

		struct Particle {
			double x, y;
			double xvel, yvel;
			double size, sized;
			sf::Color color;

			Time::WithinPeriodTimer aliveTimer;
			
			Particle(double x = 0.0f, double y = 0.0f, double xvel = 0.0f, double yvel = 0.0f, double size = 10.0f, double sized = 0.0f, double lifeTime = 1.0f, sf::Color color = sf::Color(169, 202, 30));

			bool alive();

			void tick(double dt);

		};


	}
}