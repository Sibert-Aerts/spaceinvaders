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

			loadSprite(spriteSheetTexture, playerSprite,		0, 0, 2, 1);
			loadSprite(spriteSheetTexture, playerBulletSprite1, 2, 0, 1, 1);
			loadSprite(spriteSheetTexture, playerBulletSprite2, 3, 0, 1, 1);

			loadSprite(spriteSheetTexture, enemySprite1,		0, 1, 2, 1);
			loadSprite(spriteSheetTexture, enemyBulletSprite1,	2, 1, 1, 1);
			loadSprite(spriteSheetTexture, enemyBulletSprite2,	3, 1, 1, 1);

			loadSprite(spriteSheetTexture, enemySprite2,		0, 2, 2, 1);
			loadSprite(spriteSheetTexture, lifeSprite,			2, 2, 1, 1);
			loadSprite(spriteSheetTexture, powerupSprite,		3, 2, 1, 1);

			loadSprite(spriteSheetTexture, barrierSprite1,		0, 3, 1, 1);
			loadSprite(spriteSheetTexture, barrierSprite2,		1, 3, 1, 1);
			loadSprite(spriteSheetTexture, barrierSprite3,		2, 3, 1, 1);
			loadSprite(spriteSheetTexture, barrierSprite4,		3, 3, 1, 1);
			
			// Load sounds
			loadSound(playerFireSoundBuffer, playerFireSound, "Assets\\sounds\\playerShoot.wav");
			loadSound(playerHitSoundBuffer,  playerHitSound, "Assets\\sounds\\playerHit.wav");
			loadSound(enemyFireSoundBuffer, enemyFireSound, "Assets\\sounds\\enemyShoot.wav");
			loadSound(enemyHitSoundBuffer, enemyHitSound, "Assets\\sounds\\enemyHit.wav");
			loadSound(barrierHitSoundBuffer, barrierHitSound, "Assets\\sounds\\barrierHit.wav");
			loadSound(barrierDestroySoundBuffer, barrierDestroySound, "Assets\\sounds\\barrierDestroyed.wav");
			loadSound(pauseSoundBuffer, pauseSound, "Assets\\sounds\\pause.wav");
		}

		sf::Sprite & Resources::getEnemySprite()
		{
			static bool animState = true;
			animState = animState ^ alienAnimationDelay();
			if (animState)
				return enemySprite1;
			else
				return enemySprite2;
		}

		sf::Sprite& Resources::getBarrierSprite(int health) {
			switch (health) {
			case 0:
				throw(std::runtime_error("Attempted to draw a dead barrier."));
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

		sf::Sprite & Resources::getEnemyBulletSprite(int health) {
			switch (health) {
			case 0:
				throw(std::runtime_error("Attempted to draw a dead enemy bullet."));
				// Todo: turn that into a bad_draw_error or something
			case 1:
				return enemyBulletSprite1;
			default:	// 2 or more
				return enemyBulletSprite2;

			}
		}

		sf::Sprite & Resources::getPlayerBulletSprite(int health) {
			switch (health) {
			case 0:
				throw(std::runtime_error("Attempted to draw a dead player bullet."));
				// Todo: turn that into a bad_draw_error or something
			case 1:
				return playerBulletSprite1;
			default:	// 2 or more
				return playerBulletSprite2;

			}
		}


		// Private:

		void Resources::loadSprite(sf::Texture & texture, sf::Sprite & sprite, unsigned int x, unsigned int y, unsigned int w, unsigned int h){
			sprite.setTexture(texture);
			sprite.setTextureRect(sf::IntRect(x*8, y*8, w*8, h*8));
			sprite.setScale(sf::Vector2f(5.0f, 5.0f));
		}

		void Resources::loadSound(sf::SoundBuffer & buffer, sf::Sound & sound, std::string file){

			if (!buffer.loadFromFile(file))
				throw(std::runtime_error("Failed to load sound: " + file));
			sound.setBuffer(buffer);
		}

	}
}
