#include "StdAfx.h"
#include "entity.h"
#include "payload.h"


namespace SI {
	namespace Md {

		// Helper functions:

		// A function that calculates the resulting velocity of an object after collission
		// Used for the debugEntities
		double elasticCollission(double v1, double m1, double v2, double m2) {
			return (v1*(m1 - m2) + 2 * m2*v2) / (m1 + m2);
		}

		// Entity

		Entity::Entity(EntityType type, double xpos, double ypos, float size, int health) :
			type(type), xpos(xpos), ypos(ypos), size(size), health(health) {
		}

		void Entity::registerPayloadEntity(std::shared_ptr<PayloadEntity> payloadEntity){
			this->payloadEntity = payloadEntity;
			payloadEntity->type = this->type;
			updatePosition();
			updateHealth();
		}

		void Entity::updatePosition(){
			if (!payloadEntity)
				throw(std::runtime_error("Entity does not have an assosicated PayloadEntity!"));
			payloadEntity->xpos = this->xpos;
			payloadEntity->ypos = this->ypos;
		}

		void Entity::updateHealth(){
			payloadEntity->health = this->health;
		}

		bool Entity::collide(std::shared_ptr<Entity> e) {
			double D = sqrt(pow((xpos - e->xpos), 2) + pow((ypos - e->ypos), 2));
			double d = D - (size + e->size);
			if (d > 0)
				return false;
			return true;
		}

		bool Entity::isDead(){
			return health <= 0;
		}

		// Player : Entity

		Player::Player(double x, double y, std::string name, std::shared_ptr<Time::Stopwatch> stopwatch) :
			Entity( EntityType::player, x, y, 28.0f, 3),
			name(name),
			fireCooldown(0.4f, stopwatch),
			speed(200)
		{}

		// Bullet : Entity

		inline Bullet::Bullet( EntityType type, double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size, int health) :
			Entity(type, xpos, ypos, size, health),
			xvel(xvel), yvel(yvel),
			xacc(xacc), yacc(yacc)
		{}

		void Bullet::tick(double dt) {
			xpos += xvel * dt;
			ypos += yvel * dt;
			xvel += xacc * dt;
			yvel += yacc * dt;
			updatePosition();
		}

		void Bullet::hurt(std::shared_ptr<Barrier> e){
			int min = std::min(health, e->health);
			health -= min;
			e->health -= min;
			updateHealth();
			e->updateHealth();
		}

		// EnemyBullet : Bullet

		EnemyBullet::EnemyBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size, int health) :
			Bullet( EntityType::enemyBullet, xpos, ypos, xvel, yvel, xacc, yacc, size, health)
		{}

		void EnemyBullet::hurt(std::shared_ptr<Player> e){
			health = 0;
			e->health -= 1;
			updateHealth();
			e->updateHealth();
		}

		// PlayerBullet : Bullet

		PlayerBullet::PlayerBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size, int health) :
			Bullet( EntityType::playerBullet, xpos, ypos, xvel, yvel, xacc, yacc, size, health)
		{}

		void PlayerBullet::hurt(std::shared_ptr<Enemy> e){
			int min = std::min(health, e->health);
			health -= min;
			e->health -= min;
			updateHealth();
			e->updateHealth();
		}

		// DebugEntity : Entity

		DebugEntity::DebugEntity(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size) :
			Entity(EntityType::playerBullet, xpos, ypos),
			xvel(xvel), yvel(yvel),
			xacc(xacc), yacc(yacc),
			size(size)
		{}

		void DebugEntity::tick(double dt) {
			xvel += (dt * xacc);
			yvel += (dt * yacc);
			xpos += (dt * xvel);
			ypos += (dt * yvel);
			updatePosition();
			// Jokes below
			payloadEntity->type = EntityType(RNG::RNG::getInstance()->intFromRange(0, 7));
		}

		bool DebugEntity::collide(std::shared_ptr<DebugEntity>& e) {
			double D = sqrt(pow((xpos - e->xpos), 2) + pow((ypos - e->ypos), 2));
			double d = D - (size + e->size);
			if (d > 0)
				return false;

			double tempx(xvel), tempy(yvel);
			if (xpos < e->xpos)
				xvel = elasticCollission(xvel, size*size, e->xvel, e->size*e->size);
			yvel = elasticCollission(yvel, size*size, e->yvel, e->size*e->size);
			e->xvel = elasticCollission(e->xvel, e->size*e->size, tempx, size*size);
			e->yvel = elasticCollission(e->yvel, e->size*e->size, tempy, size*size);

			// Attempt to stop them from clipping into each other
			// But mostly just resulted in them doing weird things
			// At least they didn't clip though.
			xpos += d / D * (e->xpos - xpos);
			ypos += d / D * (e->ypos - ypos);
			return true;
		}

		// Barrier : Entity

		Barrier::Barrier(double xpos, double ypos, int health) :
			Entity(EntityType::barrier, xpos, ypos, 20.0f, health) {}

		// Enemy : Entity

		Enemy::Enemy(double x, double y, int health) :
			Entity(EntityType::enemy, x, y, 40.0f, health)
		{}

}
}
