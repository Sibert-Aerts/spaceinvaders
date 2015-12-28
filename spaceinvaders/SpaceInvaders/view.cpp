#include "StdAfx.h"
#include "view.h"

namespace SI
{


	namespace Vw {

		// VARIABLES (TODO: READ THESE FROM AN INI OR SOMETHING)

		bool drawHitboxes = false;	// Draw hitboxes or not
		
		// Helper functions
		
		// Map entity shared pointers to a predefined set of colours
		// Used for drawing debugEntities in different, consistent colours
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

		// Round a value x to the closest multiple of y
		// Used for aligning to the fake pixel grid
		double align(double x, double y) {
			double fmod = std::fmod(x, y);
			if (fmod > y / 2)
				return x - fmod + y;
			return x - fmod;
		}

	// View

		View::View(double tickPeriod) :
			stopwatch(Time::GlobalStopwatch::getInstance()),
			frameTimer(tickPeriod, stopwatch),
			resources(stopwatch),
			flickerCounter(0.05f)
		{
			// Create window
			window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 720), "Space Invaders");

		}

		void View::registerPayload(std::shared_ptr<Md::Payload> payload) {
			this->payload = payload;
		}

		void View::update() {

			if (!frameTimer())
				return;
			double dt = stopwatch->tick();
			// Fortunately this is the only class that ticks the global stopwatch (I think) else stuff would go wrong

			checkWindowEvents();

			updateEvents();

			tickParticles(dt);
						
		// ________DRAWING BEGINS HERE____________________
			window->clear(sf::Color(15,15,15));
			
			// Draw the background
			window->draw(resources.backgroundSprite);

			//TODO: REPLACE THE BOTTOM FOR-LOOP CONSTRUCTION WITH THE TOP ONE
			// how: replace draw(e) with draw<type>(e)
			//		the rest is easy you retard, you can (hopefully?) then remove the
			//		goddamn entity vector pointer from the payload and all will be DAIJOBU
			//		(TL NOTE: DAIJOBU means HUNKY-DORY)

			for (std::shared_ptr<Md::PayloadEntity> e : payload->payloadEntities) {
				switch (e->type) {
				case Md::EntityType::player:
					drawPlayer(e);
					break;
				case Md::EntityType::enemy:
					drawEnemy(e);
					break;
				case Md::EntityType::playerBullet:
					drawPlayerBullet(e);
					break;
				case Md::EntityType::enemyBullet:
					drawEnemyBullet(e);
					break;
				case Md::EntityType::barrier:
					drawBarrier(e);
					break;
				case Md::EntityType::powerup:
					drawPlayer(e);

				}
			}

			drawParticles();

			// Draw the lives
			drawLives();

			// Draw the timer
			std::string timerText = "TIME: " + std::to_string(payload->secondsPassed);
			drawShadedText(timerText, 20, green3, sf::Vector2f(680, 20), 2, green1);


			// if game over, fade the screen and draw "GAME OVER"
			if (payload->gameOver) {
				window->draw(resources.pauseOverlaySprite);
				drawShadedText("GAME OVER", 80, green2, sf::Vector2f(160, 260), 5, green1);
			}
			// if paused, fade the screen and draw "PAUSED"
			else if (payload->paused) {
				window->draw(resources.pauseOverlaySprite);
				drawShadedText("PAUSED", 80, green3, sf::Vector2f(240, 260), 5);
			}

			// DEBUG TEXT:
			// Draw the framerate
			double avg = avgFps(1 / dt);
			std::string fpsText = "FPS: " + std::to_string(avg);
			drawShadedText(fpsText, 12, ((avg < 30) ? sf::Color::Red : ((avg < 60) ? sf::Color::Yellow : sf::Color::Green)), sf::Vector2f(4, 4),2);

			// Draw the entity count
			std::string entityText = "Entities: " + std::to_string(payload->entityCount);
			drawShadedText(entityText, 12, sf::Color::Yellow , sf::Vector2f(4, 16), 2);

			window->display();
		// ________DRAWING ENDS HERE______________________
		}

		void View::checkWindowEvents(){
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window->close();
			}
		}

		void View::updateEvents(){
			std::vector<Md::Event> events = payload->popEvents();

			for (auto e : events) {
				switch (e.type) {
				case Md::EventType::friendlyShotFired:
					resources.playerFireSound.play();
					break;
				case Md::EventType::enemyShotFired:
					resources.enemyFireSound.play();
					break;
				case Md::EventType::shotHit:
					makeParticleExplosion(e.x, e.y, 400, 4, 10, -30, green2, std::_Pi / 4);
					break;
				case Md::EventType::friendlyHit:
					resources.playerHitSound.play();
					makeParticleExplosion(e.x, e.y, 400, 16, 20, -40);
					break;
				case Md::EventType::enemyHit:
					resources.enemyHitSound.play();
					break;
				case Md::EventType::enemyDestroyed:
					resources.enemyDestroyedSound.play();
					makeParticleExplosion(e.x, e.y, 150, 16, 10, -5, green1);
					makeParticleExplosion(e.x, e.y, 100, 8, 15, -10, green2);
					break;
				case Md::EventType::barrierHit:
					resources.barrierHitSound.play();
					break;
				case Md::EventType::barrierDestroyed:
					resources.barrierDestroySound.play();
					makeParticleExplosion(e.x, e.y, 150, 8, 15, -6, green1, std::_Pi / 8);
					makeParticleExplosion(e.x, e.y, 100, 8, 20, -10, green0);
					break;
				case Md::EventType::paused:
					resources.pauseSound.play();
					break;
				case Md::EventType::unPaused:
					resources.pauseSound.play();
					break;
				case Md::EventType::gameOver:
					// Play a gameOver song here or something...?
					break;
				}

			}
		}

		void View::makeParticleExplosion(double x, double y, unsigned int speed, unsigned int count, double size, double sized, sf::Color color, double angle) {
			for (unsigned int i = 0; i < count; ++i)
				particles.push_back(std::make_shared<Particle>(x, y, speed*std::sin(std::_Pi*2*i/count + angle), speed * std::cos(std::_Pi * 2 * i / count + angle), size, sized, 1, color));
		}

		void View::tickParticles(double dt){
			std::vector<std::shared_ptr<Particle>> dead;
			for (auto& particle : particles) {
				if (!particle->alive())
					dead.push_back(particle);
				else
					particle->tick(dt);
			}
			// Wait with deleting the dead ones until the end, to prevent horrible things from happening
			for (auto& particle : dead) {
				particles.erase(std::remove(particles.begin(), particles.end(), particle), particles.end());
			}
		}

		// DRAW FUNCTIONS

		void View::drawParticles() {
			for (auto& p : particles) {
				float size = (float)align(p->size, 5.0f);
				drawRectangle(size, size, p->color, align(p->x - size / 2, 5.0f) , align(p->y - size / 2, 5.0f));
			}
		}

		void View::drawLives(){
			for (unsigned int i = 0; i < payload->lives; ++i)
				drawSprite(resources.lifeSprite, 320 + i * 60, 20);
		}
		
		void View::draw(std::shared_ptr<Md::DebugEntity> e) {
			//drawText(std::to_string(d->xpos) + " , " + std::to_string(d->ypos), 12, sf::Color(255,0,0), sf::Vector2f((float)d->xpos, (float)d->ypos));
			drawCircle(e->size, mapEntityToColor(e), e->xpos, e->ypos);
		}

		void View::drawSprite(sf::Sprite& sprite, double x, double y) {
			sprite.setPosition((float)align(x, 5.0f), (float)align(y,5.0f));
			window->draw(sprite);
		}

		void View::drawPlayer(std::shared_ptr<Md::PayloadEntity> e) {
			if(flickerCounter.getCount()%2 || !payload->playerInvinc)	// Don't draw the player if he's invincible and the flicker counter is in a certain state
				drawSprite(resources.playerSprite, e->xpos - 40, e->ypos - 20);
		}

		void View::drawPlayerBullet(std::shared_ptr<Md::PayloadEntity> e) {
			drawSprite(resources.getPlayerBulletSprite(e->health), e->xpos - 20, e->ypos - 20);
		}

		void View::drawEnemyBullet(std::shared_ptr<Md::PayloadEntity> e) {
			drawSprite(resources.getEnemyBulletSprite(e->health), e->xpos - 20, e->ypos - 20);
		}

		void View::drawEnemy(std::shared_ptr<Md::PayloadEntity> e) {
			drawSprite(resources.getEnemySprite(), e->xpos - 40, e->ypos - 20);
		}
		
		void View::drawBarrier(std::shared_ptr<Md::PayloadEntity> e) {
			drawSprite(resources.getBarrierSprite(e->health), e->xpos - 20, e->ypos - 20);
		}

		void View::drawText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position) {
			sf::Text t;
			t.setFont(resources.font8BitOperator);
			t.setString(text);
			t.setCharacterSize(size);
			t.setColor(color);
			t.setPosition(position);
			window->draw(t);
		}

		void View::drawShadedText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position, int shadeDistance, sf::Color shadeColor){
			drawText(text, size, shadeColor, sf::Vector2f(position.x, position.y + shadeDistance));
			drawText(text, size, color, position);
		}

		void View::drawCircle(float size, sf::Color color, double x, double y) {
			sf::CircleShape shape(size);
			shape.setFillColor(color);
			shape.setPosition(sf::Vector2f((float)x - size, (float)y - size));
			window->draw(shape);
		}	
		
		void View::drawRectangle(float width, float height, sf::Color color, double x, double y) {
			sf::RectangleShape shape(sf::Vector2f(width, height));
			shape.setFillColor(color);
			shape.setPosition(sf::Vector2f((float)x , (float)y));
			window->draw(shape);
		}
		
	}
}