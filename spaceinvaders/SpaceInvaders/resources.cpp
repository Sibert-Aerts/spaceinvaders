#include "StdAfx.h"
#include "resources.h"

namespace SI {
	namespace Vw {
		Resources::Resources(std::shared_ptr<Time::Stopwatch> stopwatch) :
			alienAnimationDelay(0.5f, stopwatch)
		{
			// Load font
			if (!font8BitOperator.loadFromFile("Assets\\8bitOperatorPlus-Bold.ttf"))
				throw(std::runtime_error("Failed to load font file: Assets\\8bitOperatorPlus-Bold.ttf"));

			// Load background
			if (!backgroundTexture.loadFromFile("Assets\\sprites\\bg.png"))
				throw(std::runtime_error("Failed to load texture: Assets\\sprites\\bg.png"));
			backgroundSprite.setTexture(backgroundTexture);
			backgroundSprite.setScale(sf::Vector2f(5.0f, 5.0f));

			// Load pause overlay
			if (!pauseOverlayTexture.loadFromFile("Assets\\sprites\\pauseOverlay.png"))
				throw(std::runtime_error("Failed to load texture: Assets\\sprites\\pauseOverlay.png"));
			pauseOverlaySprite.setTexture(pauseOverlayTexture);
			pauseOverlaySprite.setScale(sf::Vector2f(5.0f, 5.0f));

			// Load sprites
			if (!spriteSheetTexture.loadFromFile("Assets\\sprites\\sprites.png"))
				throw(std::runtime_error("Failed to load texture: Assets\\sprites\\sprites.png"));

			
			// setTexture
			playerSprite.setTexture(spriteSheetTexture);
			playerBulletSprite.setTexture(spriteSheetTexture);
			alienBulletSprite.setTexture(spriteSheetTexture);
			lifeSprite.setTexture(spriteSheetTexture);

			alienSprite1.setTexture(spriteSheetTexture);
			alienSprite2.setTexture(spriteSheetTexture);

			barrierSprite1.setTexture(spriteSheetTexture);
			barrierSprite2.setTexture(spriteSheetTexture);
			barrierSprite3.setTexture(spriteSheetTexture);
			barrierSprite4.setTexture(spriteSheetTexture);

			// setTextureRect
			playerSprite.setTextureRect(sf::IntRect(0, 0, 16, 8));
			playerBulletSprite.setTextureRect(sf::IntRect(16, 0, 8, 8));
			alienBulletSprite.setTextureRect(sf::IntRect(16, 8, 8, 8));
			lifeSprite.setTextureRect(sf::IntRect(16, 16, 8, 8));

			alienSprite1.setTextureRect(sf::IntRect(0, 8, 16, 8));
			alienSprite2.setTextureRect(sf::IntRect(0, 16, 16, 8));

			barrierSprite1.setTextureRect(sf::IntRect(0, 24, 8, 8));
			barrierSprite2.setTextureRect(sf::IntRect(8, 24, 8, 8));
			barrierSprite3.setTextureRect(sf::IntRect(16, 24, 8, 8));
			barrierSprite4.setTextureRect(sf::IntRect(24, 24, 8, 8));

			// setScale
			playerSprite.setScale(sf::Vector2f(5.0f, 5.0f));
			playerBulletSprite.setScale(sf::Vector2f(5.0f, 5.0f));
			alienBulletSprite.setScale(sf::Vector2f(5.0f, 5.0f));
			lifeSprite.setScale(sf::Vector2f(5.0f, 5.0f));

			alienSprite1.setScale(sf::Vector2f(5.0f, 5.0f));
			alienSprite2.setScale(sf::Vector2f(5.0f, 5.0f));

			barrierSprite1.setScale(sf::Vector2f(5.0f, 5.0f));
			barrierSprite2.setScale(sf::Vector2f(5.0f, 5.0f));
			barrierSprite3.setScale(sf::Vector2f(5.0f, 5.0f));
			barrierSprite4.setScale(sf::Vector2f(5.0f, 5.0f));

			// Load sounds
			loadSound(playerFireSoundBuffer, playerFireSound, "Assets\\sounds\\playerShoot.wav");
			loadSound(playerHitSoundBuffer,  playerHitSound, "Assets\\sounds\\playerHit.wav");
			loadSound(enemyFireSoundBuffer, enemyFireSound, "Assets\\sounds\\enemyShoot.wav");
			loadSound(enemyHitSoundBuffer, enemyHitSound, "Assets\\sounds\\enemyHit.wav");
			loadSound(barrierHitSoundBuffer, barrierHitSound, "Assets\\sounds\\barrierHit.wav");
			loadSound(barrierDestroySoundBuffer, barrierDestroySound, "Assets\\sounds\\barrierDestroyed.wav");
			loadSound(pauseSoundBuffer, pauseSound, "Assets\\sounds\\pause.wav");
		}

		sf::Sprite & Resources::getAlienSprite()
		{
			static bool animState = true;
			animState = animState ^ alienAnimationDelay();
			if (animState)
				return alienSprite1;
			else
				return alienSprite2;
		}

		sf::Sprite& Resources::getBarrierSprite(unsigned int health) {
			switch (health) {
			case 0:
				throw(std::runtime_error("Attempted to draw a bad barrier."));
				// Todo: turn that into a bad_draw_error or something
			case 1:
				return barrierSprite4;
			case 2:
				return barrierSprite3;
			case 3:
				return barrierSprite2;
			default:	// 4 or more
				return barrierSprite1;

			}
		}
		
		// Private:

		void Resources::loadSound(sf::SoundBuffer & buffer, sf::Sound & sound, std::string file){

			if (!buffer.loadFromFile(file))
				throw(std::runtime_error("Failed to load sound: " + file));
			sound.setBuffer(buffer);
		}

	}
}
