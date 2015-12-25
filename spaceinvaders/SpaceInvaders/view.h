#pragma once

#include "StdAfx.h"
#include "resources.h"
#include "model.h"
#include "entity.h"
#include "time.h"
#include "particle.h"
#include "tools.h"

namespace SI
{
	namespace Md {
		class Model;
		struct Payload;
	}

	namespace Vw {

		struct Resources;

		// CONSTANTS:
			// Colours
		const sf::Color green0(15, 56, 15);
		const sf::Color green1(48, 98, 48);
		const sf::Color green2(139, 172, 15);
		const sf::Color green3(169, 202, 30);

		class View {
		private:
			std::shared_ptr<sf::RenderWindow> window;

			std::shared_ptr<Md::Payload> payload;

			std::shared_ptr<Time::GlobalStopwatch> stopwatch;
			Time::BinaryRepeatTimer frameTimer;
			Time::PeriodCounter flickerCounter;
			
			Resources resources;

			std::vector<std::shared_ptr<Particle>> particles;

			// Debug:
			AverageAccumulator<double, 120> avgFps;

		public:

			View(double tickPeriod = 0);

			void registerPayload(std::shared_ptr<Md::Payload> payload);

			void update();

			void checkWindowEvents();

			void updateEvents();

			void View::makeParticleExplosion(double x, double y, unsigned int speed, unsigned int count, double size, double sized, sf::Color color = green3, double angle = 0.0f);

			void tickParticles(double dt);
			void drawParticles();

			void drawLives();

			void draw(std::shared_ptr<Md::DebugEntity> e);
			void drawSprite(sf::Sprite & sprite, double x, double y);
			void drawPlayer(std::shared_ptr<Md::PayloadEntity> e);
			void drawEnemy(std::shared_ptr<Md::PayloadEntity> e);
			void drawPlayerBullet(std::shared_ptr<Md::PayloadEntity> e);
			void drawEnemyBullet(std::shared_ptr<Md::PayloadEntity> e);
			void drawBarrier(std::shared_ptr<Md::PayloadEntity> e);
			
			void drawText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position);

			void drawShadedText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position, int shadeDistance, sf::Color shadeColor = green0);

			void drawCircle(float size, sf::Color color, double x, double y);

			void drawRectangle(float width, float height, sf::Color color, double x, double y);
		};

	}
}