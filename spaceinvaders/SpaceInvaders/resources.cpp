#include "StdAfx.h"
#include "resources.h"

namespace SI {
	namespace Vw {
		void Resources::playPlayerFireSound(){
			playerFireSound.play();
		}
		void Resources::playEnemyFireSound() {
			enemyFireSound.play();
		}
		void Resources::playPauseSound() {
			pauseSound.play();
		}
		void Resources::playPlayerHitSound() {
			playerHitSound.play();
		}
		void Resources::playEnemyHitSound() {
			enemyHitSound.play();
		}
		void Resources::playEnemyDestroyedSound() {
			enemyDestroyedSound.play();
		}
		void Resources::playBarrierHitSound() {
			barrierHitSound.play();
		}
		void Resources::playBarrierDestroySound() {
			barrierDestroySound.play();
		}
		void Resources::playPickupSound() {
			pickupSound.play();
		}

		Resources::Resources(std::shared_ptr<Time::Stopwatch> stopwatch) :
			smallEnemyAnimationTimer(0.5, stopwatch),
			bigEnemyAnimationTimer(0.2, stopwatch),
			powerupAnimationTimer(0.05, stopwatch)
		{
			// Load font
			if (!font8BitOperator.loadFromFile("Assets/8bitOperatorPlus-Bold.ttf"))
				throw(std::runtime_error("Failed to load font file: Assets/8bitOperatorPlus-Bold.ttf"));

			// Load background
			if (!backgroundTexture.loadFromFile("Assets/sprites/bg.png"))
				throw(std::runtime_error("Failed to load texture: Assets/sprites/bg.png"));
			backgroundSprite.setTexture(backgroundTexture);
			backgroundSprite.setScale(sf::Vector2f(5.0f, 5.0f));

			// Load pause overlay
			if (!pauseOverlayTexture.loadFromFile("Assets/sprites/pauseOverlay.png"))
				throw(std::runtime_error("Failed to load texture: Assets/sprites/pauseOverlay.png"));
			pauseOverlaySprite.setTexture(pauseOverlayTexture);
			pauseOverlaySprite.setScale(sf::Vector2f(5.0f, 5.0f));

			// Load sprites
			if (!spriteSheetTexture.loadFromFile("Assets/sprites/sprites.png"))
				throw(std::runtime_error("Failed to load texture: Assets/sprites/sprites.png"));

			loadSprite(spriteSheetTexture, playerSprite,		0, 0, 2, 1);
			loadSprite(spriteSheetTexture, playerBulletSprite1, 2, 0, 1, 1);
			loadSprite(spriteSheetTexture, playerBulletSprite2, 3, 0, 1, 1);
			loadSprite(spriteSheetTexture, playerBulletSprite3, 4, 0, 1, 1);
			loadSprite(spriteSheetTexture, playerBulletSprite4, 5, 0, 1, 1);

			loadSprite(spriteSheetTexture, smallEnemySprite1,	0, 1, 2, 1);
			loadSprite(spriteSheetTexture, enemyBulletSprite1,	2, 1, 1, 1);
			loadSprite(spriteSheetTexture, enemyBulletSprite2,	3, 1, 1, 1);

			loadSprite(spriteSheetTexture, smallEnemySprite2,	0, 2, 2, 1);
			loadSprite(spriteSheetTexture, lifeSprite,			2, 2, 1, 1);
			loadSprite(spriteSheetTexture, powerupSprite1,		3, 2, 1, 1);
			loadSprite(spriteSheetTexture, powerupSprite2,		4, 2, 1, 1);

			loadSprite(spriteSheetTexture, barrierSprite1,		0, 3, 1, 1);
			loadSprite(spriteSheetTexture, barrierSprite2,		1, 3, 1, 1);
			loadSprite(spriteSheetTexture, barrierSprite3,		2, 3, 1, 1);
			loadSprite(spriteSheetTexture, barrierSprite4,		3, 3, 1, 1);

			loadSprite(spriteSheetTexture, bigEnemySprite1,		0, 4, 2, 2);
			loadSprite(spriteSheetTexture, bigEnemySprite2,		2, 4, 2, 2);
			loadSprite(spriteSheetTexture, bigEnemySprite3,		4, 4, 2, 2);
			loadSprite(spriteSheetTexture, bigEnemySprite4,		6, 4, 2, 2);
			
			// Load sounds
			loadSound(playerFireSoundBuffer, playerFireSound, "Assets/sounds/playerShoot.wav");
			loadSound(playerHitSoundBuffer,  playerHitSound, "Assets/sounds/playerHit.wav");
			loadSound(enemyFireSoundBuffer, enemyFireSound, "Assets/sounds/enemyShoot.wav");
			loadSound(enemyHitSoundBuffer, enemyDestroyedSound, "Assets/sounds/enemyDestroyed.wav");
			loadSound(barrierHitSoundBuffer, barrierHitSound, "Assets/sounds/barrierHit.wav");
			loadSound(barrierDestroySoundBuffer, barrierDestroySound, "Assets/sounds/barrierDestroyed.wav");
			loadSound(pauseSoundBuffer, pauseSound, "Assets/sounds/pause.wav");
			loadSound(pickupSoundBuffer, pickupSound, "Assets/sounds/powerup.wav");

			enemyHitSound = barrierDestroySound;
			enemyHitSound.setPitch(2.0f);
			enemyHitSound.setVolume(75.0f);
		}


		sf::Font & Resources::getFont(){
			return font8BitOperator;
		}

		sf::Sprite & Resources::getBackgroundSprite(){
			return backgroundSprite;
		}

		sf::Sprite & Resources::getPauseOverlaySprite() {
			return pauseOverlaySprite;
		}

		sf::Sprite & Resources::getSmallEnemySprite()
		{
			switch(smallEnemyAnimationTimer.getCount()%2) {
			case 0:
				return smallEnemySprite1;
			case 1:
				return smallEnemySprite2;
			default:	// Will never happen because it's a modulo but else the compiler complains
				return smallEnemySprite1;
			}
		}

		sf::Sprite & Resources::getBigEnemySprite()
		{
			switch (bigEnemyAnimationTimer.getCount() % 4) {
			case 0:
				return bigEnemySprite1;
			case 1:
				return bigEnemySprite2;
			case 2:
				return bigEnemySprite3;
			case 3:
				return bigEnemySprite4;
			default:	// Will never happen because it's a modulo but else the compiler complains
				return bigEnemySprite1;
			}
		}

		sf::Sprite& Resources::getBarrierSprite(int health) {
			switch (health) {
			case 0:
				throw(std::runtime_error("Attempted to draw a dead barrier."));
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
			case 1:
				return playerBulletSprite1;
			case 2:
				return playerBulletSprite2;
			case 3:
				return playerBulletSprite3;
			default:	// 4 or more
				return playerBulletSprite4;

			}
		}

		sf::Sprite & Resources::getPlayerSprite() {
			return playerSprite;
		}

		sf::Sprite & Resources::getLifeSprite() {
			return lifeSprite;
		}

		sf::Sprite & Resources::getPowerupSprite() {
			switch (powerupAnimationTimer.getCount() % 2) {
			case 0:
				return powerupSprite1;
			default:
				return powerupSprite2;
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
