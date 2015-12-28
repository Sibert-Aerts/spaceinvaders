#include "StdAfx.h"
#include "entity.h"
#include "model.h"


namespace SI {
	namespace Md {

		// Helper functions:

		// A function that calculates the resulting velocity of an object after collission
		// Used for the debugEntities
		double elasticCollission(double v1, double m1, double v2, double m2) {
			return (v1*(m1 - m2) + 2 * m2*v2) / (m1 + m2);
		}

		// Entity

		// static data member:
		Model* Entity::model;

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
			if (!payloadEntity)
				throw(std::runtime_error("Entity does not have an assosicated PayloadEntity!"));
			payloadEntity->health = this->health;
		}

		bool Entity::hit(std::shared_ptr<Entity> e) {
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
			model->addEvent(Event(bulletHit, xpos, ypos));
			model->addEvent(Event(barrierHit, e->xpos, e->ypos));

			int min = std::min(health, e->health);
			health -= min;
			e->health -= min;

			updateHealth();
			e->updateHealth();
			if (e->isDead()) {
				model->deleteEntity(e);
				model->addEvent(Event(barrierDestroyed, e->xpos, e->ypos));
			}
		}

		// EnemyBullet : Bullet

		EnemyBullet::EnemyBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, int health) :
			Bullet( EntityType::enemyBullet, xpos, ypos, xvel, yvel, xacc, yacc, (float)(5 + health * 5), health)
		{}

		void EnemyBullet::hurt(std::shared_ptr<Player> e){
			model->addEvent(Event(bulletHit, xpos, ypos));
			health = 0;
			e->health -= 1;

			updateHealth();
			e->updateHealth();
		}

		// PlayerBullet : Bullet

		PlayerBullet::PlayerBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, int health) :
			Bullet( EntityType::playerBullet, xpos, ypos, xvel, yvel, xacc, yacc, (float)(5 + health * 5), health)
		{}

		void PlayerBullet::hurt(std::shared_ptr<Enemy> e){
			model->addEvent(Event(bulletHit, xpos, ypos));
			model->addEvent(Event(enemyHit, e->xpos, e->ypos));

			int min = std::min(health, e->health);
			health -= min;
			e->health -= min;

			updateHealth();
			e->updateHealth();

			if (e->isDead()) {
				model->deleteEntity(e);
				model->addEvent(Event(enemyDestroyed, e->xpos, e->ypos));
			}
		}

		// Barrier : Entity

		Barrier::Barrier(double xpos, double ypos, int health) :
			Entity(EntityType::barrier, xpos, ypos, 20.0f, health) {}

		// Enemy : Entity

		Enemy::Enemy(double x, double y, int health) :
			Entity(EntityType::enemy, x, y, 35.0f, health) {}

		void Enemy::tick(double dt){
			if (RNG::RNG::getInstance()->chanceOutOf(1, 4000)) {
				model->addEntity(std::make_shared<Md::EnemyBullet>(xpos, ypos, RNG::RNG::getInstance()->intFromRange(-20, 20), 300, 0, 0));
				model->addEvent(Event(EventType::enemyShotFired));
			}
		}

		void Enemy::hurt(std::shared_ptr<Barrier> e){
			health -= e->health/3;	// 1 if barrier is at 3 or 4 health, 0 if barrier is at 1 or 2 health
			model->addEvent(Event(bulletHit, (e->xpos+xpos)/2, (e->ypos + ypos) / 2));	// create an explosion between the barrier and alien
			updateHealth();
			
			model->deleteEntity(e);
			model->addEvent(Event(barrierDestroyed, e->xpos, e->ypos));

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
			payloadEntity->type = (EntityType)((payloadEntity->type + 1) % 8);
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

		// EnemyCluster

		double EnemyCluster::rightMostPoint() {
			double right = std::numeric_limits<double>::min();
			for (auto& enemy : enemies)
				right = std::max(right, enemy->xpos + enemy->size);
			return right;
		}

		double EnemyCluster::leftMostPoint(){
			double left = std::numeric_limits<double>::max();
			for (auto& enemy : enemies)
				left = std::min(left, enemy->xpos - enemy->size);
			return left;
		}

		double EnemyCluster::lowestPoint(){
			// Lowest actually means highest since y increases downwards
			double low = std::numeric_limits<double>::min();
			for (auto& enemy : enemies)
				low = std::max(low, enemy->ypos + enemy->size);
			return low;
		}

		EnemyCluster::EnemyCluster() : 
			xDir(true), yDistance(-1.0f){}

		void EnemyCluster::addEnemy(std::shared_ptr<Enemy> enemy){
			enemies.push_back(enemy);
		}

		void EnemyCluster::deleteEnemy(std::shared_ptr<Enemy> enemy){
			enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
		}

		unsigned int EnemyCluster::count(){
			return enemies.size();
		}

		void EnemyCluster::tick(double dt){
			static unsigned int initialCount = count();
			double vel = 20.0f + (initialCount - count()) * 6.0f;
			double xd(0.0f), yd(0.0f);
			if (yDistance < 0) {
				if (( xDir && rightMostPoint() > 790) || (!xDir && leftMostPoint() < 10)) {
					yDistance = 40.0f;
					xDir = !xDir;
				} else {
					xd = (xDir ? 1 : -1) * vel * dt;
				}
			}
			else {
				yd = dt * vel;
				yDistance -= yd;
			}

			for (auto& enemy : enemies){
				enemy->ypos += yd;
				enemy->xpos += xd;
				enemy->updatePosition();
			}
		}
	}
}
