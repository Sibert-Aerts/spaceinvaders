#pragma once
#include "StdAfx.h"
#include "particle.h"

namespace SI {
	namespace Vw {




		Particle::Particle(double x, double y, double xvel, double yvel, double size, double sized, double lifeTime, sf::Color color) :
			x(x), y(y), xvel(xvel), yvel(yvel), size(size), sized(sized), aliveTimer(lifeTime), color(color)
		{}

		bool Particle::alive(){
			return (aliveTimer() && size > 0);
		}

		void Particle::tick(double dt){
			if (!alive())
				return;
			x += xvel * dt;
			y += yvel * dt;
			size += sized * dt;
		}

	}
}