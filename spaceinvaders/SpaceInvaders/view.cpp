#include "StdAfx.h"
#include "view.h"

namespace SI
{


	namespace Vw {

		// VARIABLES (TODO: READ THESE FROM AN INI OR SOMETHING)

		double ghostUpdateInterval = 0.001f;			// How often the ghost layer gets updated, this is a very expensive operation right now so better be high
		double ghostDecrementInterval = 0.002f;		// How often the ghost layer alpha should get ticked down, doesn't affect performance

		// Helper functions

		void imageColorSub(sf::Image& image, sf::Color c) {
			for (unsigned int x = 0; x < image.getSize().x; ++x){
				for (unsigned int y = 0; y < image.getSize().y; ++y) {
					sf::Color p = image.getPixel(x, y);
					image.setPixel(x, y, sf::Color(	std::max(p.r - c.r, 0), std::max(p.g - c.g, 0),
													std::max(p.b - c.b, 0), std::max(p.a - c.a, 0)));

				}
			}
		}

		sf::Color mapEntityToColor(const std::shared_ptr<Md::Entity>& e) {
			static std::map<std::shared_ptr<Md::Entity>, sf::Color> map;

			static std::vector<sf::Color> colors =
			{ sf::Color(255,255,0),		sf::Color(0,255,255),	sf::Color(255,0,255),
				sf::Color(255,128,128), sf::Color(128,255,128), sf::Color(128,128,255),
				sf::Color(255,0,0),		sf::Color(0,255,0),		sf::Color(0,0,255),
				sf::Color(255,128,0),	sf::Color(0,255,128),	sf::Color(128,0,255),
				sf::Color(255,0,128),	sf::Color(128,255,0),	sf::Color(0,128,255), 
			};

			static unsigned int i(0);

			if (map.find(e) == map.end()) {
				map[e] = colors[i];
				i = (i + 1) % colors.size();
			}
			
			return map[e];
		}

	// View

		View::View(int w, int h, const char* name, double tickPeriod) :
			stopwatch(Time::GlobalStopwatch::getInstance()),
			frameTimer(tickPeriod, stopwatch),
			ghostUpdateTimer(ghostUpdateInterval, stopwatch),
			ghostDecrementCounter(ghostDecrementInterval, stopwatch)
		{
			// Create window
			window = std::make_shared<sf::RenderWindow>(sf::VideoMode(w, h), name);

			// Load font
			if (!font8BitOperator.loadFromFile("Assets\\8bitOperatorPlus-Bold.ttf")) {
				throw(std::runtime_error("Failed to load font file: 8bitOperatorPlus-Bold.ttf"));
			}

			// Create ghost texture
			ghosts.create(800, 600);
		}

		void View::update(std::vector<std::shared_ptr<Md::Entity>>& entities) {

			if (!frameTimer())
				return;
			double dt = stopwatch->tick();

			checkWindowEvents();
			
		// ________DRAWING BEGINS HERE____________________
			window->clear(sf::Color::Black);

			// Draw a debug circle to test transparency
			drawCircle(120.0f, sf::Color(60, 60, 120), 400, 300);

			// Tick and draw ghosts, spooky stuff
			
			/*
			if (ghostUpdateTimer() && ghostDecrementCounter())
				tickGhosts();
			ghosts.display();
			sf::Sprite ghostSprite(ghosts.getTexture());
			window->draw(ghostSprite);
			*/

			// Draw the given entities
			for (std::shared_ptr<Md::Entity> entity : entities) {
				if (auto e = std::dynamic_pointer_cast<Md::DebugEntity>(entity)) {
					draw(e);
				}
				else if (auto e = std::dynamic_pointer_cast<Md::Player>(entity)) {
					draw(e);
				}
				else if (auto e = std::dynamic_pointer_cast<Md::Bullet>(entity)) {
					draw(e);
				}
				else if (auto e = std::dynamic_pointer_cast<Md::Enemy>(entity)) {
					draw(e);
				}
			}


			// Draw the framerate
			std::string fpsText = "FPS: " + std::to_string(avgFps(1/dt));
			drawText(fpsText, 12, sf::Color::Black, sf::Vector2f(6, 6));
			drawText(fpsText, 12, ((1 / dt < 30) ? sf::Color::Red : ((1 / dt < 60) ? sf::Color::Yellow : sf::Color::Green)), sf::Vector2f(4, 4));

			// Draw the entity count
			std::string entityText = "Entities: " + std::to_string(entities.size());
			drawText(entityText, 12, sf::Color::Black, sf::Vector2f(6, 18));
			drawText(entityText, 12, sf::Color::Yellow , sf::Vector2f(4, 16));

			window->display();
		// ________DRAWING ENDS HERE______________________
		}

		void View::tickGhosts() {
			// Jump through hoops to subtract the ghost alpha by 1
			sf::Image oldGhosts = ghosts.getTexture().copyToImage();
			unsigned int i = ghostDecrementCounter.count() + 1;
			imageColorSub(oldGhosts, sf::Color(0, 0, 0, i));
			sf::Texture t;
			t.loadFromImage(oldGhosts);
			ghosts.clear(sf::Color(0, 0, 0, 0));
			ghosts.draw(sf::Sprite(t), sf::BlendNone);	// BlendNone to prevent bad alpha composition
		}

		void View::checkWindowEvents(){
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window->close();
			}
		}

		// DRAW FUNCTIONS
		void View::draw(std::shared_ptr<Md::DebugEntity> e) {
			//drawText(std::to_string(d->xpos) + " , " + std::to_string(d->ypos), 12, sf::Color(255,0,0), sf::Vector2f((float)d->xpos, (float)d->ypos));
			drawCircle(e->size, mapEntityToColor(e), e->xpos, e->ypos, true);
		}

		void View::draw(std::shared_ptr<Md::Player> e) {
			drawText(e->name, 12, sf::Color(255, 0, 0), sf::Vector2f((float)e->xpos, (float)e->ypos - 40));
			drawCircle(20.0f, sf::Color::Red, e->xpos, e->ypos, true);
		}

		void View::draw(std::shared_ptr<Md::Bullet> e) {
			drawCircle(e->size, sf::Color::White, e->xpos, e->ypos, true);
		}

		void View::draw(std::shared_ptr<Md::Enemy> e) {
			drawCircle(e->size, sf::Color(120,40,40), e->xpos, e->ypos, true);
		}
		
		void View::drawText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position) {
			sf::Text t;
			t.setFont(font8BitOperator);
			t.setString(text);
			t.setCharacterSize(size);
			t.setColor(color);
			t.setPosition(position);
			window->draw(t);
		}

		void View::drawCircle(float size, sf::Color color, double x, double y, bool ghosted){
			sf::CircleShape shape(size);
			shape.setFillColor(color);
			shape.setPosition(sf::Vector2f((float)x-size, (float)y-size));
			window->draw(shape);
			if (ghosted)
				ghosts.draw(shape);
		}
		
	}
}