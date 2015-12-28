#pragma once
#include "StdAfx.h"
#include "time.h"

namespace SI {
	namespace Vw {

		// A struct that loads/stores the various resources the view needs
		// So that the View isn't a big mess
		struct Resources {

			// Fonts:
			sf::Font font8BitOperator;

			// Sprites:
			sf::Sprite backgroundSprite;
			sf::Sprite pauseOverlaySprite;

			sf::Sprite enemySprite1;
			sf::Sprite enemySprite2;

			sf::Sprite& getEnemySprite();

			sf::Sprite barrierSprite1;
			sf::Sprite barrierSprite2;
			sf::Sprite barrierSprite3;
			sf::Sprite barrierSprite4;

			sf::Sprite& getBarrierSprite(int health);

			sf::Sprite enemyBulletSprite1;
			sf::Sprite enemyBulletSprite2;

			sf::Sprite& getEnemyBulletSprite(int health);

			sf::Sprite playerBulletSprite1;
			sf::Sprite playerBulletSprite2;

			sf::Sprite& getPlayerBulletSprite(int health);

			sf::Sprite playerSprite;
			sf::Sprite lifeSprite;

			sf::Sprite powerupSprite;

			Time::BinaryRepeatTimer alienAnimationDelay;

			// Sounds:
			sf::Sound playerFireSound;
			sf::Sound enemyFireSound;
			sf::Sound pauseSound;
			sf::Sound playerHitSound;
			sf::Sound enemyHitSound;
			sf::Sound enemyDestroyedSound;
			sf::Sound barrierHitSound;
			sf::Sound barrierDestroySound;

			Resources(std::shared_ptr<Time::Stopwatch> stopwatch = Time::GlobalStopwatch::getInstance());
		
		private:
			// Textures:
			sf::Texture backgroundTexture;
			sf::Texture pauseOverlayTexture;
			sf::Texture spriteSheetTexture;

			// Soundbuffers:
			sf::SoundBuffer playerFireSoundBuffer;
			sf::SoundBuffer enemyFireSoundBuffer;
			sf::SoundBuffer pauseSoundBuffer;
			sf::SoundBuffer playerHitSoundBuffer;
			sf::SoundBuffer enemyHitSoundBuffer;
			sf::SoundBuffer barrierHitSoundBuffer;
			sf::SoundBuffer barrierDestroySoundBuffer;

			// functions
			void loadSprite(sf::Texture& texture, sf::Sprite& sprite, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
			void loadSound(sf::SoundBuffer& buffer, sf::Sound& sound, std::string file);


		};

	}
}