#pragma once

#include "StdAfx.h"
#include "model.h"
#include "entity.h"
#include "time.h"
#include "tools.h"

namespace SI
{
	namespace Md {
		class Model;
		struct Payload;
	}

	namespace Vw {

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

			// ASSETS
			sf::Font font8BitOperator;
			sf::Texture backgroundTexture;
			sf::Sprite backgroundSprite;
			sf::Texture spriteSheetTexture;
			sf::Sprite alienSprite1;
			sf::Sprite alienSprite2;
			sf::Sprite alienBulletSprite;
			sf::Sprite playerBulletSprite;
			sf::Sprite playerSprite;

			Time::BinaryRepeatTimer alienAnimationDelay;

			sf::RenderTexture ghosts;
			Time::BinaryRepeatTimer ghostUpdateTimer;
			Time::CountingRepeatTimer ghostDecrementCounter;

			// Debug:
			AverageAccumulator<double, 15> avgFps;

		public:

			View(int w, int h, const char* name, double tickPeriod = 0);

			void registerPayload(std::shared_ptr<Md::Payload> payload);

			void update();

			void checkWindowEvents();

			void draw(std::shared_ptr<Md::DebugEntity> e);
			void drawSprite(sf::Sprite & sprite, double x, double y);
			void draw(std::shared_ptr<Md::Player> e);
			void draw(std::shared_ptr<Md::PlayerBullet> e);
			void draw(std::shared_ptr<Md::EnemyBullet> e);
			void draw(std::shared_ptr<Md::Enemy> e);

			void tickGhosts();

			void drawText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position);

			void drawShadedText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position, int shadeDistance, sf::Color shadeColor = green0);

			void drawCircle(float size, sf::Color color, double x, double y, bool ghosted = false);

			void drawRectangle(float width, float height, sf::Color color, double x, double y, bool ghosted);
		};

	}
}