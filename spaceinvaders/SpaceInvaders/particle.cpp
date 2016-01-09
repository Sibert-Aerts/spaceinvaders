#pragma once
#include "StdAfx.h"
#include "particle.h"

namespace SI {
	namespace Vw {


		// Particle 

		Particle::Particle(double x, double y, double xvel, double yvel, double size, double sized, double lifeTime, sf::Color color) :
			x(x), y(y), xvel(xvel), yvel(yvel), size(size), sized(sized), aliveTimer(lifeTime), color(color)
		{}

		bool Particle::alive(){
			return (aliveTimer() && size > 0);
		}

		double Particle::getX() const {
			return x;
		}

		double Particle::getY() const {
			return y;
		}

		double Particle::getSize() const {
			return size;
		}

		sf::Color Particle::getColor() const {
			return color;
		}

		void Particle::tick(double dt){
			if (!alive())
				return;
			x += xvel * dt;
			y += yvel * dt;
			size += sized * dt;
		}

		// TextParticle : Particle

		TextParticle::TextParticle(std::string text, double x, double y, double xvel, double yvel, double lifeTime, sf::Color color) : 
			Particle(x, y, xvel, yvel, 10.0, 0.0, lifeTime, color), text(text)
		{}

		std::string TextParticle::getText() const {
			return text;
		}
		
	}
}