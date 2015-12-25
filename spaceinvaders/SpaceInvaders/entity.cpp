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

		Entity::Entity(EntityType type, double xpos, double ypos, float size) : 
			type(type), xpos(xpos), ypos(ypos), size(size) {}

		void Entity::registerPayloadEntity(std::shared_ptr<PayloadEntity> payloadEntity){
			this->payloadEntity = payloadEntity;
			payloadEntity->type = this->type;
		}

		void Entity::updatePosition(){
			if (!payloadEntity)
				throw(std::runtime_error("Entity does not have an assosicated PayloadEntity!"));
			payloadEntity->xpos = this->xpos;
			payloadEntity->ypos = this->ypos;
		}

		bool Entity::collide(std::shared_ptr<Entity> e) {
			double D = sqrt(pow((xpos - e->xpos), 2) + pow((ypos - e->ypos), 2));
			double d = D - (size + e->size);
			if (d > 0)
				return false;
			return true;
		}

		// Player : Entity

		Player::Player(double x, double y, std::string name, std::shared_ptr<Time::Stopwatch> stopwatch) :
			Entity( EntityType::player, x, y, 28.0f),
			name(name),
			fireCooldown(0.4f, stopwatch),
			lives(3),
			speed(200)
		{}

		// Bullet : Entity

		inline Bullet::Bullet( EntityType type, double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size) :
			Entity(type, xpos, ypos),
			xvel(xvel), yvel(yvel),
			xacc(xacc), yacc(yacc),
			size(size)
		{}

		void Bullet::tick(double dt) {
			xpos += xvel * dt;
			ypos += yvel * dt;
			xvel += xacc * dt;
			yvel += yacc * dt;
			updatePosition();
		}

		// EnemyBullet : Bullet

		EnemyBullet::EnemyBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size) :
			Bullet( EntityType::enemyBullet, xpos, ypos, xvel, yvel, xacc, yacc, size)
		{}

		// PlayerBullet : Bullet

		PlayerBullet::PlayerBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, float size) :
			Bullet( EntityType::playerBullet, xpos, ypos, xvel, yvel, xacc, yacc, size)
		{}

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

		Barrier::Barrier(double xpos, double ypos, unsigned int health) :
			Entity(EntityType::barrier, xpos, ypos, 20.0f), health(health) {}
		
		// Enemy : Entity

		Enemy::Enemy(double x, double y, float size) :
			Entity(EntityType::enemy, x, y, size)
		{}

}
}
