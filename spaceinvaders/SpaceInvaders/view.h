#pragma once

#include "StdAfx.h"
#include "resources.h"
#include "model.h"
#include "time.h"
#include "particle.h"
#include "tools.h"

namespace SI
{
	namespace Md {
		class Model;
		class ModelObserver;
	}

	namespace Vw {

		struct Resources;

		// CONSTANTS:
			// Colours: The 4 colours of our palette
		extern const sf::Color green0;
		extern const sf::Color green1;
		extern const sf::Color green2;
		extern const sf::Color green3;

		class View {
		private:
			// The window to which things get drawn
			std::shared_ptr<sf::RenderWindow> window;

			// The observer which observes our model, and contains the information we need
			std::shared_ptr<Md::ModelObserver> observer;

			// The RNG provider
			std::shared_ptr<RNG::RNG> rng;

			// The global timer
			std::shared_ptr<Time::GlobalStopwatch> stopwatch;

			// A timer that determines the minimum time between frames
			Time::BinaryRepeatTimer frameTimer;

			// A timer that determines the frequency of flickering animations
			Time::PeriodCounter flickerCounter;
			
			// A class that loads, stores and provides the various texture and sound resources
			Resources resources;

			// A vector of particles
			std::vector<std::shared_ptr<Particle>> particles;

			// Debug:
			// A simple class that keeps track of the average framerate out of every 120 samples
			AverageAccumulator<double, 120> avgFps;


		public:
			// Create a view with a certain minimum period between frames
			View(double tickPeriod = 0.0);

			// Get a pointer to the observer so it can be registered in a model
			std::shared_ptr<Md::ModelObserver> getObserver();

			// Draw everything based off the observer's data
			void update();

			// Check window events, so the window can close properly
			void checkWindowEvents();

			// Perform actions based off the events stored in the observer
			void drawEvents();

			// Create a circular set of particles to simulate an explosion
			void makeParticleExplosion(double x, double y, double speed, unsigned int count, double size, double sized, sf::Color color = green3, double angle = 0.0, double time = 1.0);

			// Create a randomised set of particles to simulate an explosion
			void makeRandomParticleExplosion(double x, double y, double speed, double speedVar, unsigned int count, double size, double sizeVar, double sized, sf::Color color, double time, double timeVar);

			void makeTextParticle(std::string text, double x, double y);

			// Tick every particle
			void tickParticles(double dt);
			
			// Drawing functions: 
				// Particles:
			void drawParticles();
			
				// Hud:
			void drawLives();

				// Entities:
			void drawSprite(sf::Sprite & sprite, double x, double y);
			void drawPlayer(std::shared_ptr<Md::EntityObserver> e);
			void drawSmallEnemy(std::shared_ptr<Md::EntityObserver> e);
			void drawBigEnemy(std::shared_ptr<Md::EntityObserver> e);
			void drawPlayerBullet(std::shared_ptr<Md::EntityObserver> e);
			void drawEnemyBullet(std::shared_ptr<Md::EntityObserver> e);
			void drawBarrier(std::shared_ptr<Md::EntityObserver> e);
			void drawPowerup(std::shared_ptr<Md::EntityObserver> e);
			
				// Text:
			void drawText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position);
			void drawShadedText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position, int shadeDistance, sf::Color shadeColor = green0);
			void drawCenteredText(std::string text, unsigned int size, double ypos, sf::Color color);
			void drawCenteredShadedText(std::string text, unsigned int size, double ypos, sf::Color color, sf::Color shade, int shadeDistance);
			
				// Shapes:
			void drawRectangle(float width, float height, sf::Color color, double x, double y);
		};

	}
}