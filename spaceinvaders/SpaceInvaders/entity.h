#pragma once

#include "StdAfx.h"
#include "Time.h"
#include "payload.h"
#include "random.h"
#include "model.h"

namespace SI
{
	namespace Md {

		// Resolve circular dependencies with included headers
		class Model;
		struct PayloadEntity;
		enum EntityType;

		// Resolve circular dependencies within this header
		class Entity;
		class Player;
		class Enemy;
		class EnemyCluster;
		class Barrier;
		class Bullet;
		class FriendlyBullet;
		class EnemyBullet;
		class Powerup;
		
	// Classes:

		// An abstract class for an Entity in the world
		class Entity {
		public:
			static Model* model;

			double xpos, ypos;
			float size;
			int health;
			EntityType type;
			std::shared_ptr<PayloadEntity> payloadEntity;

		public:
			Entity(EntityType type, double xpos = 0, double ypos = 0, float size = 10.0f, int health = 1);

			virtual void registerPayloadEntity(std::shared_ptr<PayloadEntity> payloadEntity);

			void updatePosition();
			void updateHealth();

			virtual bool hit(std::shared_ptr<Entity> e);

			virtual bool isDead();

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
			Time::BinaryRepeatTimer fireCooldown;
			double speed;
			double bulletSpeed;
			int bulletDmg;

			Player(double x, double y, std::string name, std::shared_ptr<Time::Stopwatch> stopwatch);

			void shoot();

			void moveLeft(double dt);
			void moveRight(double dt);

		};

		// An enemy
		class Enemy : public Entity {
		protected:
			std::shared_ptr<RNG::RNG> rng;

		public:
			Enemy(EntityType type, double x, double y, int health);

			void tick(double dt);

			virtual void shoot() = 0;
			virtual void hurt(std::shared_ptr<Barrier> e) = 0;
			virtual void destroyEvent() = 0;
		};

		class SmallEnemy : public Enemy {
		public:
			SmallEnemy(double x, double y, int health = 1);

			virtual void shoot();
			virtual void hurt(std::shared_ptr<Barrier> e);
			virtual void destroyEvent();
		};

		class BigEnemy : public Enemy {
		public:
			BigEnemy(double x, double y, int health = 2);

			virtual void shoot();
			virtual void hurt(std::shared_ptr<Barrier> e);
			virtual void destroyEvent();
		};
		
		// A cluster of enemies
		class EnemyCluster {
		private:
			Model* model;

			std::vector<std::shared_ptr<Enemy>> enemies;
			unsigned int initialCount;

			double speed, speedInc;

			bool xDir;
			double yDistance;

			double rightMostPoint();
			double leftMostPoint();

		public:
			EnemyCluster();

			void setSpeed(double speed, double speedInc);

			void addEnemy(std::shared_ptr<Enemy> enemy);
			void deleteEnemy(std::shared_ptr<Enemy> enemy);
			void clear();

			unsigned int count();
			double lowestPoint();

			void tick(double dt);

		};

		// A barrier
		class Barrier : public Entity {
		public:
			Barrier(double xpos, double ypos, int health = 4);
		};

		// An abstract bullet
		class Bullet : public Entity {
		public:
			double xvel, yvel;
			double xacc, yacc;

			Bullet(EntityType type, double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size = 10.0f, int health = 1);
			
			void tick(double dt);

			void hurt(std::shared_ptr<Barrier> e);

		};

		// A friendly bullet
		class PlayerBullet : public Bullet {
		public:
			PlayerBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, int health = 2);

			void hurt(std::shared_ptr<Enemy> e);
		};
		
		// An enemy bullet
		class EnemyBullet : public Bullet {
		public:
			EnemyBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, int health = 1);

			void hurt(std::shared_ptr<Player> e);
		};
	}
}