#pragma once

#include "StdAfx.h"
#include "time.h"
#include "observer.h"
#include "random.h"
#include "model.h"

namespace SI
{
	namespace Md {

		// Declare all entities first to resolve some dependencies
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
		protected:
			// The parent Model which the Entity is part of
			Model* model;

			// The Entity's coordinates in the world
			double xpos, ypos;
			// The Entity's diameter
			float size;
			// The Entity's health value
			int health;

			// The type of entity it should be drawn as
			EntityType type;

			// The Entity's observers
			std::vector<std::shared_ptr<EntityObserver>> observers;

		public:
			// Initialise an Entity with a type, xpos, ypos, size and health
			Entity(EntityType type, double xpos = 0, double ypos = 0, float size = 10.0f, int health = 1);

			// Register the Model this entity is a part of, so the entity can spawn entities
			void registerModel(Model* model);

			// Register an EntityObserver
			void registerObserver(std::shared_ptr<EntityObserver>& observer);

			// Get a reference to the observers, used for deleting observers
			const std::vector<std::shared_ptr<EntityObserver>>& getObservers() const;
			
			// Get the x coordinate
			double getX() const;
			// Set the x coordinate
			void setX(double xpos);

			// Get the y coordinate
			double getY() const;
			// Set the y coordinate
			void setY(double ypos);

			// Get the entity's diameter
			float getSize() const;
			
			// Get the health value
			int getHealth() const;
			// Set the health value
			void setHealth(int health);

			// Update the observer with the current position
			void updatePosition();
			// Updatre the observer with the current health value
			void updateHealth();

			// Check whether or not this entity touches another entity
			virtual bool hit(const std::shared_ptr<Entity> e) const;

			// Check whether or not the health value is 0 or less
			virtual bool isDead() const;

		};
	
		// An entity representing the player
		class Player : public Entity {
		protected:
			// A timer ensuring a minimum time between firing
			Time::BinaryRepeatTimer fireCooldown;

			// The Player's speed
			const double speed;
			// The Player's additional speed
			double speedUpVal;
			// The speed of the Player's bullets
			const double bulletSpeed;
			// The additional speed of the Player's bullets
			double bulletSpeedUpVal;
			// The damage of the Player's bullets
			const int bulletDmg;
			// The additional damage of the Player's bullets
			int bulletDmgUpVal;

		public:
			Player(double x, double y, std::shared_ptr<Time::Stopwatch> stopwatch);

			// Increase the Player's speed
			void speedUp();
			// Increase the speed of the Player's bullets
			void bulletSpeedUp();
			// Increase the damage of the Player's bullets
			void damageUp();
			// Increase the Player's fire rate
			void fireRateUp();
			// Reset all of the Player's powerups
			void resetPowerups();

			// Shoot a bullet
			void shoot();

			// Move to the left
			void moveLeft(double dt);

			// Move to the right
			void moveRight(double dt);

		};

		// An enemy
		class Enemy : public Entity {
		protected:
			// A pointer to the RNG singleton
			std::shared_ptr<RNG::RNG> rng;

		public:
			Enemy(EntityType type, double x, double y, int health);
			
			// Fire a bullet depending on random chance and the amount of time passed
			void tick(double dt);

			// Fire an enemy bullet
			virtual void shoot() = 0;

			// Hurt a barrier and possibly the Enemy itself
			virtual void hurt(std::shared_ptr<Barrier> e) = 0;

			// Push back a specific Event and possibly spawn a Powerup
			virtual void destroyEvent() = 0;
		};

		// A small enemy
		class SmallEnemy : public Enemy {
		public:
			SmallEnemy(double x, double y, int health = 1);

			// Fire a small and fast EnemyBullet
			virtual void shoot();

			// Hurt a Barrier and the Enemy based off both their health values
			// May leave the Barrier and/or the Enemy dead
			virtual void hurt(std::shared_ptr<Barrier> e);

			// Push back a specific Event and possibly spawn a Powerup
			virtual void destroyEvent();
		};

		// A large enemy
		class BigEnemy : public Enemy {
		public:
			BigEnemy(double x, double y, int health = 2);

			// Fire a large and slow EnemyBullet
			virtual void shoot();

			// Destroy the Barrier without taking damage
			// Leaves the Barrier dead but the Enemy unharmed
			virtual void hurt(std::shared_ptr<Barrier> e);

			// Push back a specific Event and possibly spawn a Powerup
			virtual void destroyEvent();
		};
		
		// A cluster of enemies
		class EnemyCluster {
		private:
			// The parent Model
			Model* model;

			// A timer determining whether the EnemyCluster is allowed to move or not
			Time::WithinPeriodTimer frozen;

			// The enemies part of this cluster
			std::vector<std::shared_ptr<Enemy>> enemies;

			// The initial number of enemies in the cluster
			unsigned int initialCount;

			// The speed of the enemy cluster, and the increment in speed whenever an enemy is killed
			double speed, speedInc;

			// The direction the cluster is heading in: left or right
			bool xDir;

			// The amount of distance left to travel downwards
			double yDistance;

			// Find the rightmost position of any enemy within the cluster
			double rightMostPoint();

			// Find the leftmost position of any enemy within the cluster
			double leftMostPoint();

		public:
			EnemyCluster(std::shared_ptr<Time::Stopwatch> stopwatch);

			// Set the cluster's speed values during level loading
			void setSpeed(double speed, double speedInc);

			// Register an enemy in the cluster
			void addEnemy(std::shared_ptr<Enemy> enemy);

			// Unregister an enemy from the cluster
			void deleteEnemy(std::shared_ptr<Enemy> enemy);

			// Unregister all enemies, reset the direction values
			void clear();

			// Return the current number of enemies
			unsigned int count();

			// Find the lowest position of any enemy within the cluster
			double lowestPoint();

			// Advance the cluster's position depending on the time passed
			void tick(double dt);

			// Freeze the enemies for a period of time
			void freeze();

		};

		// A Barrier which blocks bullets and enemies
		class Barrier : public Entity {
		public:
			Barrier(double xpos, double ypos, int health = 4);
		};

		// An abstract bullet
		class Bullet : public Entity {
		public:
			// The velocity of the bullet
			double xvel, yvel;

			// The acceleration of the bullet
			double xacc, yacc;

			Bullet(EntityType type, double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size = 10.0f, int health = 1);
			
			// Advance the Bullet's state depending on the time passed, velocity and acceleration
			void tick(double dt);

			// Hurt a Barrier depending on the health values from both
			// Capable of leaving the Bullet and/or the Barrier dead
			void hurt(std::shared_ptr<Barrier> e);

		};

		// A friendly bullet
		class PlayerBullet : public Bullet {
		public:
			PlayerBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, int health = 2);

			// Hurt an enemy based on the health values of both
			// Can leave the bullet and/or the enemy dead
			void hurt(std::shared_ptr<Enemy> e);
		};
		
		// An enemy bullet
		class EnemyBullet : public Bullet {
		public:
			EnemyBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, int health = 1);

			// Hurt the player for 1 health, and destroy yourself
			// Leaves the bullet dead and may leave the player dead
			void hurt(std::shared_ptr<Player> e);
		};

		// A type of powerup
		enum PowerupType {
			slowdown, speedUp, fireRateUp, bulletSpeedUp, damageUp
		};

		// A powerup that can be picked up by the player
		class Powerup : public Entity {
		protected:
			// The drop velocity
			double yvel;
		public:
			Powerup(double xpos, double ypos);

			// Get the randomly decided type of powerup
			PowerupType getPowerupType();

			// Advance the powerup's drop
			void tick(double dt);
		};
	}
}
