#pragma once
#include "StdAfx.h"
#include "time.h"
#include "tools.h"

namespace SI {
	namespace Vw {

		class Particle {
		protected:
			double x, y;
			double xvel, yvel;
			double size, sized;
			sf::Color color;

			Time::WithinPeriodTimer aliveTimer;
			
		public:
			Particle(double x = 0.0f, double y = 0.0f, double xvel = 0.0f, double yvel = 0.0f, double size = 10.0f, double sized = 0.0f, double lifeTime = 1.0f, sf::Color color = sf::Color(169, 202, 30));
			
			bool alive();

			double getX();
			double getY();
			double getSize();
			sf::Color getColor();

			virtual void tick(double dt);
		};

		class TextParticle : public Particle {
		protected:
			std::string text;
		public:
			TextParticle(std::string text, double x = 0.0f, double y = 0.0f, double xvel = 0.0f, double yvel = 0.0f, double lifeTime = 1.0f, sf::Color color = sf::Color(169, 202, 30));
	
			std::string getText();
		};


	}
}