#include "StdAfx.h"
#include "view.h"

namespace SI
{


	namespace Vw {

		// VARIABLES (TODO: READ THESE FROM AN INI OR SOMETHING)

		double ghostUpdateInterval = 0.001f;		// How often the ghost layer gets updated, this is a very expensive operation right now so better be high
		double ghostDecrementInterval = 0.002f;		// How often the ghost layer alpha should get ticked down, doesn't affect performance
		bool drawHitboxes = false;					// Draw hitboxes or not
		
		// Helper functions

		// Subtract a color from an image (used for crappy implementation of ghosting)
		void imageColorSub(sf::Image& image, sf::Color c) {
			for (unsigned int x = 0; x < image.getSize().x; ++x){
				for (unsigned int y = 0; y < image.getSize().y; ++y) {
					sf::Color p = image.getPixel(x, y);
					image.setPixel(x, y, sf::Color(	std::max(p.r - c.r, 0), std::max(p.g - c.g, 0),
													std::max(p.b - c.b, 0), std::max(p.a - c.a, 0)));

				}
			}
		}

		// Map entity shared pointers to a predefined set of colours
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
			ghostDecrementCounter(ghostDecrementInterval, stopwatch),
			alienAnimationDelay(0.5f, stopwatch)
		{
			// Create window
			window = std::make_shared<sf::RenderWindow>(sf::VideoMode(w, h), name);

			// Load font
			if (!font8BitOperator.loadFromFile("Assets\\8bitOperatorPlus-Bold.ttf"))
				throw(std::runtime_error("Failed to load font file: Assets\\8bitOperatorPlus-Bold.ttf"));

			// Load background
			if(!backgroundTexture.loadFromFile("Assets\\sprites\\bg.png"))
				throw(std::runtime_error("Failed to load texture: Assets\\sprites\\bg.png"));
			backgroundSprite.setTexture(backgroundTexture);
			backgroundSprite.setScale(sf::Vector2f(5.0f, 5.0f));

			// Load sprites
			if (!spriteSheetTexture.loadFromFile("Assets\\sprites\\sprites.png"))
				throw(std::runtime_error("Failed to load texture: Assets\\sprites\\sprites.png"));
			playerSprite.setTexture(spriteSheetTexture);
			playerSprite.setTextureRect(sf::IntRect(0, 0, 16, 8));
			playerBulletSprite.setTexture(spriteSheetTexture);
			playerBulletSprite.setTextureRect(sf::IntRect(16, 0, 8, 8));
			alienBulletSprite.setTexture(spriteSheetTexture);
			alienBulletSprite.setTextureRect(sf::IntRect(16, 8, 8, 8));
			alienSprite1.setTexture(spriteSheetTexture);
			alienSprite1.setTextureRect(sf::IntRect(0, 8, 16, 8));
			alienSprite2.setTexture(spriteSheetTexture);
			alienSprite2.setTextureRect(sf::IntRect(0, 16, 16, 8));

			playerSprite.setScale(sf::Vector2f(5.0f, 5.0f));
			playerBulletSprite.setScale(sf::Vector2f(5.0f, 5.0f));
			alienBulletSprite.setScale(sf::Vector2f(5.0f, 5.0f));
			alienSprite1.setScale(sf::Vector2f(5.0f, 5.0f));
			alienSprite2.setScale(sf::Vector2f(5.0f, 5.0f));

			// Create ghost texture
			ghosts.create(800, 720);
		}

		void View::registerPayload(std::shared_ptr<Md::Payload> payload) {
			this->payload = payload;
		}

		void View::update() {

			if (!frameTimer())
				return;
			double dt = stopwatch->tick();

			checkWindowEvents();
			
		// ________DRAWING BEGINS HERE____________________
			window->clear(sf::Color(15,15,15));
			
			// Draw the background
			window->draw(backgroundSprite);

			// Tick and draw ghosts, spooky stuff
			/*
			if (ghostUpdateTimer() && ghostDecrementCounter())
				tickGhosts();
			ghosts.display();
			sf::Sprite ghostSprite(ghosts.getTexture());
			window->draw(ghostSprite);
			*/
			
			// Draw the given entities
			for (std::shared_ptr<Md::Entity> entity : *(payload->entities)) {
				if (auto e = std::dynamic_pointer_cast<Md::DebugEntity>(entity)) {
					draw(e);
				}
				else if (auto e = std::dynamic_pointer_cast<Md::Player>(entity)) {
					draw(e);
				}
				else if (auto e = std::dynamic_pointer_cast<Md::PlayerBullet>(entity)) {
					draw(e);
				}
				else if (auto e = std::dynamic_pointer_cast<Md::EnemyBullet>(entity)) {
					draw(e);
				}
				else if (auto e = std::dynamic_pointer_cast<Md::Enemy>(entity)) {
					draw(e);
				}
			}


			// Draw the lives
			std::string livesText = "LIVES: " + std::to_string(payload->lives);
			drawShadedText(livesText, 20, green3, sf::Vector2f(320, 20), 2, green1);

			// Draw the timer
			std::string timerText = "TIME: " + std::to_string(payload->secondsPassed);
			drawShadedText(timerText, 20, green3, sf::Vector2f(680, 20), 2, green1);


			// if game over, fade the screen and draw "GAME OVER"
			if (payload->gameOver) {
				drawRectangle(800, 720, sf::Color(0, 20, 0, 220), 0.0f, 0.0f, false);
				drawRectangle(800, 120, green1, 0.0f, 250.0f, false);
				drawShadedText("GAME OVER", 80, green2, sf::Vector2f(160, 260), 5);
			}
			// if paused, fade the screen and draw "PAUSED"
			else if (payload->paused) {
				drawRectangle(800, 720, sf::Color(0, 20, 0, 200), 0.0f, 0.0f, false);
				drawRectangle(800, 120, green1, 0.0f, 250.0f, false);
				drawShadedText("PAUSED", 80, green3, sf::Vector2f(240, 260), 5);
			}

			// DEBUG TEXT:
			// Draw the framerate
			std::string fpsText = "FPS: " + std::to_string(avgFps(1/dt));
			drawShadedText(fpsText, 12, ((1 / dt < 30) ? sf::Color::Red : ((1 / dt < 60) ? sf::Color::Yellow : sf::Color::Green)), sf::Vector2f(4, 4),2);

			// Draw the entity count
			std::string entityText = "Entities: " + std::to_string(payload->entities->size());
			drawShadedText(entityText, 12, sf::Color::Yellow , sf::Vector2f(4, 16), 2);

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

		void View::drawSprite(sf::Sprite& sprite, double x, double y) {
			sprite.setPosition((float)x, (float)y);
			window->draw(sprite);
		}

		void View::draw(std::shared_ptr<Md::Player> e) {
			drawSprite(playerSprite, e->xpos - 40, e->ypos - 20);
			if (drawHitboxes)
				drawCircle(e->size, sf::Color(255, 0, 0, 128), e->xpos, e->ypos, true);
		}

		void View::draw(std::shared_ptr<Md::PlayerBullet> e) {
			drawSprite(playerBulletSprite, e->xpos - 20, e->ypos - 20);
			if (drawHitboxes)
				drawCircle(e->size, sf::Color(255, 0, 0, 128), e->xpos, e->ypos, true);
		}

		void View::draw(std::shared_ptr<Md::EnemyBullet> e) {
			drawSprite(alienBulletSprite, e->xpos - 20, e->ypos - 20);
			if (drawHitboxes)
				drawCircle(e->size, sf::Color(255, 0, 0, 128), e->xpos, e->ypos, true);
		}

		void View::draw(std::shared_ptr<Md::Enemy> e) {
			static bool animState;
			animState = animState ^ alienAnimationDelay();
			if(animState)
				drawSprite(alienSprite1, e->xpos - 40, e->ypos - 20);
			else
				drawSprite(alienSprite2, e->xpos - 40, e->ypos - 20);
			if(drawHitboxes)
				drawCircle(e->size, sf::Color(255, 0, 0, 128), e->xpos, e->ypos, true);
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

		void View::drawShadedText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position, int shadeDistance, sf::Color shadeColor){
			drawText(text, size, shadeColor, sf::Vector2f(position.x + shadeDistance, position.y + shadeDistance));
			drawText(text, size, color, position);
		}

		void View::drawCircle(float size, sf::Color color, double x, double y, bool ghosted) {
			sf::CircleShape shape(size);
			shape.setFillColor(color);
			shape.setPosition(sf::Vector2f((float)x - size, (float)y - size));
			window->draw(shape);
			if (ghosted)
				ghosts.draw(shape);
		}	
		
		void View::drawRectangle(float width, float height, sf::Color color, double x, double y, bool ghosted) {
			sf::RectangleShape shape(sf::Vector2f(width, height));
			shape.setFillColor(color);
			shape.setPosition(sf::Vector2f((float)x , (float)y));
			window->draw(shape);
			if (ghosted)
				ghosts.draw(shape);
		}
		
	}
}