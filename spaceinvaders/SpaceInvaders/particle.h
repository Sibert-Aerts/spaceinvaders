#pragma once
#include "StdAfx.h"
#include "time.h"
#include "tools.h"

namespace SI {
	namespace Vw {

		class Particle {
		protected:
			// The coordinates of the particle
			double x, y;
			// The particle's velocity
			double xvel, yvel;
			// The particle's size and size gained per second
			double size, sized;
			// The particle's color
			sf::Color color;

			// Timer determining the particle's lifetime
			Time::WithinPeriodTimer aliveTimer;
			
		public:
			Particle(double x = 0.0f, double y = 0.0f, double xvel = 0.0f, double yvel = 0.0f, double size = 10.0f, double sized = 0.0f, double lifeTime = 1.0f, sf::Color color = sf::Color(169, 202, 30));
			
			// Get whether or not the particle is still alive
			bool alive();

			// Get the particle's x position
			double getX() const;
			// Get the particle's y position
			double getY() const;
			// Get the particle's size
			double getSize() const;
			// Get the particle's color
			sf::Color getColor() const;

			virtual void tick(double dt);
		};

		// A particle representing a piece of text
		class TextParticle : public Particle {
		protected:
			std::string text;
		public:
			TextParticle(std::string text, double x = 0.0f, double y = 0.0f, double xvel = 0.0f, double yvel = 0.0f, double lifeTime = 1.0f, sf::Color color = sf::Color(169, 202, 30));

			// Get the text particle's text
			std::string getText() const;
		};


	}
}