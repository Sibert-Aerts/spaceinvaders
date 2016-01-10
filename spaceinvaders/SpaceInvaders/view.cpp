#include "StdAfx.h"
#include "view.h"

namespace SI
{
	namespace Vw {

		// Debug: Whether or not the debug overlay should be drawn
		bool drawDebugTextSetting = false;

		// Constant variables

		const sf::Color green0(15, 56, 15);
		const sf::Color green1(48, 98, 48);
		const sf::Color green2(139, 172, 15);
		const sf::Color green3(169, 202, 30);
		
		const double pi(3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844);
		
	// View

		View::View(double tickPeriod) :
			stopwatch(Time::GlobalStopwatch::getInstance()),
			frameTimer(tickPeriod, stopwatch),
			resources(stopwatch),
			flickerCounter(0.05f),
			rng(RNG::RNG::getInstance())
		{
			// Create window
			window = std::make_shared<sf::RenderWindow>(sf::VideoMode(800, 720), "Space Invaders");
			observer = std::make_shared<Md::ModelObserver>();
		}

		std::shared_ptr<Md::ModelObserver> View::getObserver() {
			return observer;
		}

		void View::update() {

			if (!frameTimer())
				return;

			double dt = stopwatch->tick();

			// Check if the window is closed
			checkWindowEvents();

			checkEvents();

			tickParticles(dt);
						
		// ________DRAWING BEGINS HERE____________________________________
			window->clear();
			
				// Game elements:
			// Draw the background
			window->draw(resources.getBackgroundSprite());
			
			// Draw the entities
			for (std::shared_ptr<Md::EntityObserver> e : observer->getEntityObservers()) {
				switch (e->getType()) {
				case Md::EntityType::player:
					drawPlayer(e);
					break;
				case Md::EntityType::smallEnemy:
					drawSmallEnemy(e);
					break;
				case Md::EntityType::bigEnemy:
					drawBigEnemy(e);
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
					drawPowerup(e);
				}
			}

			// Draw the particles
			drawParticles();

				// HUD elements:
			// Draw the lives
			drawLives();

			// Draw the timer
			std::string timerText = "TIME: " + std::to_string(observer->getSecondsPassed());
			drawShadedText(timerText, 20, green3, sf::Vector2f(660, 20), 2, green1);

			// Draw an overlay if the game isn't running
			switch (observer->getState()) {
			case Md::ModelState::running:
				break;
			case Md::ModelState::paused:
				window->draw(resources.getPauseOverlaySprite());
				drawCenteredShadedText(observer->getLevelName(), 50, 160, green3, green0, 5);
				drawCenteredShadedText("PAUSED", 80, 300, green2, green1, 5);
				drawCenteredText("Press escape to unpause", 30, 360, green1);
				break;
			case Md::ModelState::gameOver:
				window->draw(resources.getPauseOverlaySprite());
				drawCenteredShadedText("GAME OVER", 80, 300, green2, green1, 5);
				drawCenteredText("Press escape to retry", 30, 360, green1);
				break;
			case Md::ModelState::levelSwitch:
				window->draw(resources.getPauseOverlaySprite());
				drawCenteredShadedText("LEVEL COMPLETE", 80, 300, green2, green1, 5);
				drawCenteredText("Next level: " + observer->getLevelName(), 30, 360, green1);
				break;
			case Md::ModelState::victory:
				window->draw(resources.getPauseOverlaySprite());
				drawCenteredShadedText("GAME COMPLETE", 80, 300, green2, green1, 5);
				drawCenteredText("Press escape to restart", 30, 360, green1);
				break;
			}

			// DEBUG TEXT:
			if (drawDebugTextSetting) {
				drawDebugText(dt);
			}

			window->display();
		// ________DRAWING ENDS HERE______________________________________
		}

		void View::checkWindowEvents(){
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window->close();
			}
		}

		void View::checkEvents(){
			std::vector<Md::Event> events = observer->popEvents();
			for (auto e : events) {
				switch (e.getType()) {
				case Md::EventType::friendlyShotFired:
					resources.playPlayerFireSound();
					break;
				case Md::EventType::enemyShotFired:
					resources.playEnemyFireSound();
					break;
				case Md::EventType::bulletHit:
					makeParticleExplosion(e.getX(), e.getY(), 400, 4, 10, -30, green2, pi / 4);
					break;
				case Md::EventType::friendlyHit:
					resources.playPlayerHitSound();
					makeParticleExplosion(e.getX(), e.getY(), 400, 16, 20, -40);
					break;
				case Md::EventType::enemyHit:
					resources.playEnemyHitSound();
					break;
				case Md::EventType::smallEnemyDestroyed:
					resources.playEnemyDestroyedSound();
					makeParticleExplosion(e.getX(), e.getY(), 150, 16, 10, -5, green1);
					makeParticleExplosion(e.getX(), e.getY(), 100, 8, 15, -10, green2);
					break;
				case Md::EventType::bigEnemyDestroyed:
					resources.playEnemyDestroyedSound();
					makeRandomParticleExplosion(e.getX(), e.getY(), 75, 50, 16, 20, 5, -15, green0, 3.0, 0.0);
					makeRandomParticleExplosion(e.getX(), e.getY(), 250, 100, 8, 3, 0, 0, green3, 1.0, 0.3);
					makeParticleExplosion(e.getX(), e.getY(), 150, 16, 12, -5, green1, 0.0, 1.2);
					makeParticleExplosion(e.getX(), e.getY(), 75, 8, 25, -20, green2, 0.0, 1.5);
					break;
				case Md::EventType::barrierHit:
					resources.playBarrierHitSound();
					break;
				case Md::EventType::barrierDestroyed:
					resources.playBarrierDestroySound();
					makeParticleExplosion(e.getX(), e.getY(), 150, 8, 15, -6, green1, pi / 8);
					makeParticleExplosion(e.getX(), e.getY(), 100, 8, 20, -10, green0);
					break;
				case Md::EventType::pickup:
					makeParticleExplosion(e.getX(), e.getY(), 600, 4, 10, -30, green3);
					makeTextParticle(e.getText(), e.getX(), e.getY());
					resources.playPickupSound();
					break;
				case Md::EventType::paused:
					resources.playPauseSound();
					break;
				case Md::EventType::unPaused:
					resources.playPauseSound();
					break;
				case Md::EventType::gameOver:
					// Play a gameOver song here or something...?
					break;
				}
			}
		}

		void View::makeParticleExplosion(double x, double y, double speed, unsigned int count, double size, double sized, sf::Color color, double angle, double time) {
			for (unsigned int i = 0; i < count; ++i)
				particles.push_back(std::make_shared<Particle>(x, y, speed*std::sin(pi*2*i/count + angle), speed * std::cos(pi * 2 * i / count + angle), size, sized, time, color));
		}

		void View::makeRandomParticleExplosion(double x, double y, double speed, double speedVar, unsigned int count, double size, double sizeVar, double sized, sf::Color color, double time, double timeVar){
			for (unsigned int i = 0; i < count; ++i) {
				double rSpeed = rng->realFromRange(speed-speedVar, speed+speedVar);
				double angle = rng->realFromRange(0.0, (double)pi * 2);
				double rSize = rng->realFromRange(size - sizeVar, size + sizeVar);
				double rTime = rng->realFromRange(time - timeVar, time + timeVar);
				particles.push_back(std::make_shared<Particle>(x, y, rSpeed*std::sin(angle), rSpeed * std::cos(angle), rSize, sized, rTime, color));
				}
			}

		void View::makeTextParticle(std::string text, double x, double y){
			particles.push_back(std::make_shared<TextParticle>(text, x, y, 0.0, -40.0, 2.0, green3));
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
				if (const auto& e = std::dynamic_pointer_cast<TextParticle>(p)) {
					drawShadedText(e->getText(), 40, e->getColor(), sf::Vector2f((float)e->getX(), (float)e->getY()), 5);
				}
				else {
					float size = (float)align(p->getSize(), 5.0f);
					drawRectangle(size, size, p->getColor(), align(p->getX() - size / 2, 5.0f), align(p->getY() - size / 2, 5.0f));
				}
			}
		}

		void View::drawLives(){
			int lives = observer->getLives();
			if (lives < 0 )
				throw(std::runtime_error("Attempted to draw negative lives."));
			for (int i = 0; i < lives; ++i)
				drawSprite(resources.getLifeSprite(), 320 + i * 60, 20);
		}

		void View::drawSprite(sf::Sprite& sprite, double x, double y) {
			sprite.setPosition((float)align(x, 5.0f), (float)align(y,5.0f));
			window->draw(sprite);
		}

		void View::drawPlayer(std::shared_ptr<Md::EntityObserver> e) {
			if((flickerCounter.getCount()%2 || !observer->isPlayerInvinc() )&& !observer->isPlayerDead() )	
				// Don't draw the player if he's dead
				// Don't draw the player if he's invincible and the flicker state is on
				drawSprite(resources.getPlayerSprite(), e->getXpos() - 40, e->getYpos() - 20);
		}

		void View::drawPlayerBullet(std::shared_ptr<Md::EntityObserver> e) {
			drawSprite(resources.getPlayerBulletSprite(e->getHealth()), e->getXpos()- 20, e->getYpos() - 20);
		}

		void View::drawEnemyBullet(std::shared_ptr<Md::EntityObserver> e) {
			drawSprite(resources.getEnemyBulletSprite(e->getHealth()), e->getXpos() - 20, e->getYpos() - 20);
		}

		void View::drawSmallEnemy(std::shared_ptr<Md::EntityObserver> e) {
			drawSprite(resources.getSmallEnemySprite(), e->getXpos() - 40, e->getYpos() - 20);
		}

		void View::drawBigEnemy(std::shared_ptr<Md::EntityObserver> e){
			drawSprite(resources.getBigEnemySprite(), e->getXpos() - 40, e->getYpos() - 40);
		}
		
		void View::drawBarrier(std::shared_ptr<Md::EntityObserver> e) {
			drawSprite(resources.getBarrierSprite(e->getHealth()), e->getXpos() - 20, e->getYpos() - 20);
		}

		void View::drawPowerup(std::shared_ptr<Md::EntityObserver> e){
			drawSprite(resources.getPowerupSprite(), e->getXpos() - 20, e->getYpos() - 20);
		}

		void View::drawText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position) {
			sf::Text t;
			t.setFont(resources.getFont());
			t.setString(text);
			t.setCharacterSize(size);
			t.setColor(color);
			t.setPosition(position);
			window->draw(t);
		}

		void View::drawShadedText(std::string text, unsigned int size, sf::Color color, sf::Vector2f position, int shadeDistance, sf::Color shadeColor){
			sf::Text t;
			t.setFont(resources.getFont());
			t.setString(text);
			t.setCharacterSize(size);

			t.setColor(shadeColor);
			t.setPosition(sf::Vector2f(position.x, position.y + shadeDistance));
			window->draw(t);

			t.setColor(color);
			t.setPosition(position);
			window->draw(t);
		}

		void View::drawCenteredText(std::string text, unsigned int size, double ypos, sf::Color color){
			sf::Text t;
			t.setFont(resources.getFont());
			t.setString(text);
			t.setCharacterSize(size);

			sf::FloatRect textRect = t.getLocalBounds();
			t.setOrigin(textRect.left + textRect.width / 2.0f,
				textRect.top + textRect.height / 2.0f);
			
			t.setColor(color);
			t.setPosition(sf::Vector2f(800 / 2.0f, (float)ypos));
			window->draw(t);
		}

		void View::drawCenteredShadedText(std::string text, unsigned int size, double ypos, sf::Color color, sf::Color shade, int shadeDistance) {
			sf::Text t;
			t.setFont(resources.getFont());
			t.setString(text);
			t.setCharacterSize(size);
			t.setColor(shade);

			sf::FloatRect textRect = t.getLocalBounds();
			t.setOrigin(textRect.left + textRect.width / 2.0f,
				textRect.top + textRect.height / 2.0f);

			t.setPosition(sf::Vector2f(800 / 2.0f, (float)ypos + shadeDistance));
			window->draw(t);

			t.setColor(color);
			t.setPosition(sf::Vector2f(800 / 2.0f, (float)ypos));
			window->draw(t);
		}

		void View::drawRectangle(float width, float height, sf::Color color, double x, double y) {
			sf::RectangleShape shape(sf::Vector2f(width, height));
			shape.setFillColor(color);
			shape.setPosition(sf::Vector2f((float)x , (float)y));
			window->draw(shape);
		}

		void View::drawDebugText(double dt){
			// Draw the framerate
			double avg = avgFps(1 / dt);
			std::string text = "FPS: " + std::to_string(avg);
			drawShadedText(text, 12, ((avg < 30) ? sf::Color::Red : ((avg < 60) ? sf::Color::Yellow : sf::Color::Green)), sf::Vector2f(4, 4), 2);

			// Draw the entity count
			text = "Entities: " + std::to_string(observer->getEntityCount());
			drawShadedText(text, 12, sf::Color::Yellow, sf::Vector2f(4, 16), 2);

			// Draw the entity count
			text = "Entity Observers: " + std::to_string(observer->getEntityObservers().size());
			drawShadedText(text, 12, sf::Color::Yellow, sf::Vector2f(4, 28), 2);

			// Draw the particle count
			text = "Particles: " + std::to_string(particles.size());
			drawShadedText(text, 12, sf::Color::Yellow, sf::Vector2f(4, 40), 2);
		}
		
	}
}
