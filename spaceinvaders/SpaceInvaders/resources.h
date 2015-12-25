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

			sf::Sprite alienSprite1;
			sf::Sprite alienSprite2;

			sf::Sprite barrierSprite1;
			sf::Sprite barrierSprite2;
			sf::Sprite barrierSprite3;
			sf::Sprite barrierSprite4;

			sf::Sprite& getAlienSprite();

			sf::Sprite& getBarrierSprite(unsigned int health);

			sf::Sprite alienBulletSprite;
			sf::Sprite playerBulletSprite;
			sf::Sprite playerSprite;
			sf::Sprite lifeSprite;

			Time::BinaryRepeatTimer alienAnimationDelay;

			// Sounds:
			sf::Sound playerFireSound;
			sf::Sound enemyFireSound;
			sf::Sound pauseSound;
			sf::Sound playerHitSound;
			sf::Sound enemyHitSound;
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
			void loadSound(sf::SoundBuffer& buffer, sf::Sound& sound, std::string file);


		};

	}
}