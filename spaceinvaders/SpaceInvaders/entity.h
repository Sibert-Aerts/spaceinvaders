#pragma once

#include "StdAfx.h"
#include "Time.h"

namespace SI
{
	namespace Md {
		
		struct PayloadEntity;
		enum EntityType;

	// Classes:

	// An abstract class for an Entity in the world
		class Entity {
		public:
			double xpos, ypos;
			float size;
			EntityType type;
			std::shared_ptr<PayloadEntity> payloadEntity;

		public:
			Entity(EntityType type, double xpos = 0, double ypos = 0, float size = 10.0f);

			void registerPayloadEntity(std::shared_ptr<PayloadEntity> payloadEntity);

			void updatePosition();

			virtual bool collide(std::shared_ptr<Entity> e);

		};

	// An entity that bounces around and collides with other DebugEntity
	// Implemented to test the view-model system
		class DebugEntity : public Entity {
		public:
			double xvel, yvel;
			double xacc, yacc;
			float size;

		public:
			DebugEntity(double xpos = 0, double ypos = 0, double xvel = 0, double yvel = 0, double xacc = 0, double yacc = 0, float size = 20.0f);

			virtual void tick(double dt);

			virtual bool collide(std::shared_ptr<DebugEntity>& e);

		};
	

	// An entity representing the player
		class Player : public Entity {
		public:
			std::string name;
			unsigned int lives;
			Time::BinaryRepeatTimer fireCooldown;
			double speed;

			Player(double x, double y, std::string name, std::shared_ptr<Time::Stopwatch> stopwatch);
		};

	// An enemy
		class Enemy : public Entity {
		public:
			Enemy(double x, double y, float size = 40.0f);
		};

	// An abstract bullet
		class Bullet : public Entity {
		public:
			double xvel, yvel;
			double xacc, yacc;
			float size;



			Bullet(EntityType type, double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size = 10.0f);



			void tick(double dt);

		};

	// A friendly bullet
		class PlayerBullet : public Bullet {
		public:
			PlayerBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size = 10.0f);

		};
		
	// An enemy bullet
		class EnemyBullet : public Bullet {
		public:
			EnemyBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size = 10.0f);
		};

	// A barrier
		class Barrier : public Entity {
		public:
			unsigned int health;

			Barrier(double xpos, double ypos, unsigned int health = 3u);

		};
	}
}