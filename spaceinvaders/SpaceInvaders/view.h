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
	}

	namespace Vw {

		struct EntityView {
			double xpos;
			double ypos;
			float size;
			sf::Color color;
			EntityView(double x, double y, float s, sf::Color color) : xpos(x), ypos(y), size(s), color(color) {}
		};

		class View {
		private:
			std::shared_ptr<sf::RenderWindow> window;

			std::shared_ptr<Time::GlobalStopwatch> stopwatch;
			Time::BinaryRepeatTimer frameTimer;

			sf::Font font8BitOperator;

			sf::RenderTexture ghosts;
			Time::BinaryRepeatTimer ghostUpdateTimer;
			Time::CountingRepeatTimer ghostDecrementCounter;

			AverageAccumulator<double, 15> avgFps;

		public:

			View(int w, int h, const char* name, double tickPeriod = 0);

			void update(std::vector<std::shared_ptr<Md::Entity>>& entities);

			void checkWindowEvents();

			void draw(std::shared_ptr<Md::DebugEntity> e);
			void draw(std::shared_ptr<Md::Player> e);
			void draw(std::shared_ptr<Md::Bullet> e);
			void draw(std::shared_ptr<Md::Enemy> e);

			void tickGhosts();

			void drawText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position);

			void drawCircle(float size, sf::Color color, double x, double y, bool ghosted = false);
		};

	}
}