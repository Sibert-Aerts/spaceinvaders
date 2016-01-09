#pragma once
#include "StdAfx.h"
#include "time.h"

namespace SI {
	namespace Vw {

		// A struct that loads/stores the various resources the View needs
		// To keep the View organised
		struct Resources {

			Resources(std::shared_ptr<Time::Stopwatch> stopwatch = Time::GlobalStopwatch::getInstance());

			// Fonts:
			sf::Font& getFont();

			// Sprites:
			sf::Sprite& getBackgroundSprite();
			sf::Sprite& getPauseOverlaySprite();

			sf::Sprite& getSmallEnemySprite();
			sf::Sprite& getBigEnemySprite();

			sf::Sprite& getBarrierSprite(int health);

			sf::Sprite& getEnemyBulletSprite(int health);
			sf::Sprite& getPlayerBulletSprite(int health);

			sf::Sprite& getPlayerSprite();
			sf::Sprite& getLifeSprite();

			sf::Sprite& getPowerupSprite();

			// Sounds:
			void playPlayerFireSound();
			void playEnemyFireSound();
			void playPauseSound();
			void playPlayerHitSound();
			void playEnemyHitSound();
			void playEnemyDestroyedSound();
			void playBarrierHitSound();
			void playBarrierDestroySound();
			void playPickupSound();
		
		private:
			// Fonts:
			sf::Font font8BitOperator;

			// Textures:
			sf::Texture backgroundTexture;
			sf::Texture pauseOverlayTexture;
			sf::Texture spriteSheetTexture;

			// Sprites:
			sf::Sprite backgroundSprite;
			sf::Sprite pauseOverlaySprite;

			sf::Sprite smallEnemySprite1;
			sf::Sprite smallEnemySprite2;

			sf::Sprite bigEnemySprite1;
			sf::Sprite bigEnemySprite2;
			sf::Sprite bigEnemySprite3;
			sf::Sprite bigEnemySprite4;

			sf::Sprite barrierSprite1;
			sf::Sprite barrierSprite2;
			sf::Sprite barrierSprite3;
			sf::Sprite barrierSprite4;

			sf::Sprite enemyBulletSprite1;
			sf::Sprite enemyBulletSprite2;

			sf::Sprite playerBulletSprite1;
			sf::Sprite playerBulletSprite2;
			sf::Sprite playerBulletSprite3;
			sf::Sprite playerBulletSprite4;

			sf::Sprite playerSprite;
			sf::Sprite lifeSprite;

			sf::Sprite powerupSprite1;
			sf::Sprite powerupSprite2;

			// Timers:
			Time::PeriodCounter smallEnemyAnimationTimer;
			Time::PeriodCounter bigEnemyAnimationTimer;
			Time::PeriodCounter powerupAnimationTimer;


			// Soundbuffers:
			sf::SoundBuffer playerFireSoundBuffer;
			sf::SoundBuffer enemyFireSoundBuffer;
			sf::SoundBuffer pauseSoundBuffer;
			sf::SoundBuffer playerHitSoundBuffer;
			sf::SoundBuffer enemyHitSoundBuffer;
			sf::SoundBuffer barrierHitSoundBuffer;
			sf::SoundBuffer barrierDestroySoundBuffer;
			sf::SoundBuffer pickupSoundBuffer;

			// Sounds:
			sf::Sound playerFireSound;
			sf::Sound enemyFireSound;
			sf::Sound pauseSound;
			sf::Sound playerHitSound;
			sf::Sound enemyHitSound;
			sf::Sound enemyDestroyedSound;
			sf::Sound barrierHitSound;
			sf::Sound barrierDestroySound;
			sf::Sound pickupSound;

			// Load a sprite by its texture, coordinates and width
			void loadSprite(sf::Texture& texture, sf::Sprite& sprite, unsigned int x, unsigned int y, unsigned int w, unsigned int h);

			// Load a sound by its filename
			void loadSound(sf::SoundBuffer& buffer, sf::Sound& sound, std::string file);
			
		};

	}
}