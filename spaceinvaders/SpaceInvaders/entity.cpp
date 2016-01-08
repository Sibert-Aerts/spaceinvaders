#include "StdAfx.h"
#include "entity.h"


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

		void Entity::registerModel(Model * model){
			this->model = model;
		}

		void Entity::registerObserver(std::shared_ptr<EntityObserver>& observer){
			observers.push_back(observer);
			observer->setType(type);
			updatePosition();
			updateHealth();
		}

		std::vector<std::shared_ptr<EntityObserver>>& Entity::getObservers(){
			return observers;
		}

		double Entity::getX() {
			return xpos;
		}

		double Entity::getY() {
			return ypos;
		}

		void Entity::setX(double xpos) {
			this->xpos = xpos;
		}

		void Entity::setY(double ypos) {
			this->ypos = ypos;
		}

		float Entity::getSize() {
			return size;
		}

		int Entity::getHealth() {
			return health;
		}

		void Entity::setHealth(int health){
			this->health = health;
		}

		void Entity::updatePosition(){
			if (observers.empty())
				throw(std::runtime_error("Entity does not have any associated observers!"));
			for( auto& observer : observers)
				observer->setPos(xpos, ypos);
		}

		void Entity::updateHealth(){
			if (observers.empty())
				throw(std::runtime_error("Entity does not have any associated observers!"));
			for (auto& observer : observers)
				observer->setHealth(health);
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

		Player::Player(double x, double y, std::shared_ptr<Time::Stopwatch> stopwatch) :
			Entity( EntityType::player, x, y, 28.0f, 3),
			fireCooldown(0.4, stopwatch),
			speed(200), 
			bulletSpeed(400.0), bulletDmg(1)
		{}
		
		void Player::speedUp(){
			speedUpVal += 50.0;
		}

		void Player::bulletSpeedUp(){
			bulletSpeedUpVal += 200.0;
		}

		void Player::damageUp()	{
			bulletDmgUpVal++;
		}

		void Player::fireRateUp(){
			fireCooldown.setPeriod(fireCooldown.getPeriod()*0.75);
		}

		void Player::resetPowerups(){
			speedUpVal = 0;
			bulletSpeedUpVal = 0;
			bulletDmgUpVal = 0;
			fireCooldown.setPeriod(0.4);
		}

		void Player::shoot(){
			if (fireCooldown()) {
				model->addEntity(std::make_shared<Md::PlayerBullet>(xpos, ypos, 0, -(bulletSpeed + bulletSpeedUpVal), 0, -200, bulletDmg + bulletDmgUpVal));
				model->addEvent(Event(friendlyShotFired));
			}
		}

		void Player::moveLeft(double dt){
			if (xpos < 50){
				xpos = 50.0f;
				return;
			}
			xpos -= dt * (speed + speedUpVal);
			updatePosition();
		}

		void Player::moveRight(double dt){
			if (xpos > 750) {
				xpos = 750.0f;
				return;
			}
			xpos += dt * (speed + speedUpVal);
			updatePosition();
		}

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
			model->addEvent(Event(barrierHit, e->getX(), e->getY()));

			int min = std::min(health, e->getHealth());
			health -= min;
			e->setHealth(e->getHealth() - min);

			updateHealth();
			e->updateHealth();
			if (e->isDead()) {
				model->deleteEntity(e);
				model->addEvent(Event(barrierDestroyed, e->getX(), e->getY()));
			}
		}

		// EnemyBullet : Bullet

		EnemyBullet::EnemyBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, int health) :
			Bullet( EntityType::enemyBullet, xpos, ypos, xvel, yvel, xacc, yacc, (float)(5 + health * 5), health)
		{}

		void EnemyBullet::hurt(std::shared_ptr<Player> e){
			model->addEvent(Event(bulletHit, xpos, ypos));
			health = 0;
			e->setHealth(e->getHealth() - 1);

			updateHealth();
			e->updateHealth();
		}

		// PlayerBullet : Bullet

		PlayerBullet::PlayerBullet(double xpos, double ypos, double xvel, double yvel, double xacc, double yacc, int health) :
			Bullet( EntityType::playerBullet, xpos, ypos, xvel, yvel, xacc, yacc, (float)(5 + health * 5), health)
		{}

		void PlayerBullet::hurt(std::shared_ptr<Enemy> e){
			model->addEvent(Event(bulletHit, xpos, ypos));
			model->addEvent(Event(enemyHit, e->getX(), e->getY()));

			int min = std::min(health, e->getHealth());
			health -= min;
			e->setHealth(e->getHealth() - 1);

			updateHealth();
			e->updateHealth();

			if (e->isDead()) {
				model->deleteEntity(e);
				e->destroyEvent();
			}
		}

		// Barrier : Entity

		Barrier::Barrier(double xpos, double ypos, int health) :
			Entity(EntityType::barrier, xpos, ypos, 20.0f, health) {}

		// Enemy : Entity

		Enemy::Enemy(EntityType type, double x, double y, int health) :
			Entity(type, x, y, 35.0f, health), rng(RNG::RNG::getInstance()) {}

		void Enemy::tick(double dt){
			if (rng->chanceOutOf(0.1 * dt)) {
				shoot();
			}
		}

		// SmallEnemy : Enemy

		SmallEnemy::SmallEnemy(double x, double y, int health) : Enemy(smallEnemy, x, y, health) {}

		void SmallEnemy::shoot() {
			model->addEntity(std::make_shared<Md::EnemyBullet>(xpos, ypos, rng->intFromRange(-30, 30), 300, 0, 0, 1));
			model->addEvent(Event(EventType::enemyShotFired));
		}

		void SmallEnemy::hurt(std::shared_ptr<Barrier> e){
			int min = std::min(health, 1 + e->getHealth() / 2);	// Take at least 1 damage, + 1 damage for every 2 hp the barrier has
			health -= min;
			model->addEvent(Event(bulletHit, (e->getX()+xpos)/2, (e->getY() + ypos) / 2));	// create an explosion between the barrier and alien
			updateHealth();
			
			e->setHealth(e->getHealth() - min * 2);
			if (e->isDead()) {
				model->deleteEntity(e);
				model->addEvent(Event(barrierDestroyed, e->getX(), e->getY()));
			}
			e->updateHealth();
		}

		void SmallEnemy::destroyEvent(){
			model->addEvent(Event(smallEnemyDestroyed, xpos, ypos));
			if (rng->chanceOutOf(1, 8))
				model->addEntity(std::make_shared<Md::Powerup>(xpos, ypos));
		}

		// BigEnemy : Enemy

		BigEnemy::BigEnemy(double x, double y, int health) : Enemy(bigEnemy, x, y, health) {}

		void BigEnemy::shoot() {
			model->addEntity(std::make_shared<Md::EnemyBullet>(xpos, ypos, rng->intFromRange(-20, 20), 200, 0, 0, 2));
			model->addEvent(Event(EventType::enemyShotFired));
		}

		void BigEnemy::hurt(std::shared_ptr<Barrier> e){
			// Straight-up destroy the barrier without taking damage
			model->addEvent(Event(bulletHit, (e->getX() + xpos) / 2, (e->getY() + ypos) / 2));	// create an explosion between the barrier and alien
			model->deleteEntity(e);
			model->addEvent(Event(barrierDestroyed, e->getX(), e->getY()));
		}

		void BigEnemy::destroyEvent(){
			model->addEvent(Event(bigEnemyDestroyed, xpos, ypos));
			if (rng->chanceOutOf(1, 2))
				model->addEntity(std::make_shared<Md::Powerup>(xpos, ypos));
		}

		// EnemyCluster

		double EnemyCluster::rightMostPoint() {
			double right = std::numeric_limits<double>::min();
			for (auto& enemy : enemies)
				right = std::max(right, enemy->getX() + enemy->getSize());
			return right;
		}

		double EnemyCluster::leftMostPoint(){
			double left = std::numeric_limits<double>::max();
			for (auto& enemy : enemies)
				left = std::min(left, enemy->getX() - enemy->getSize());
			return left;
		}

		double EnemyCluster::lowestPoint(){
			// Lowest actually means highest since y increases downwards
			double low = std::numeric_limits<double>::min();
			for (auto& enemy : enemies)
				low = std::max(low, enemy->getY() + enemy->getSize());
			return low;
		}

		EnemyCluster::EnemyCluster(std::shared_ptr<Time::Stopwatch> stopwatch) :
			xDir(true), yDistance(-1.0f), initialCount(0), frozen(3.0, true, stopwatch){}

		void EnemyCluster::setSpeed(double speed, double speedInc){
			this->speed = speed;
			this->speedInc = speedInc;
		}

		void EnemyCluster::addEnemy(std::shared_ptr<Enemy> enemy){
			enemies.push_back(enemy);
		}

		void EnemyCluster::deleteEnemy(std::shared_ptr<Enemy> enemy){
			enemies.erase(std::remove(enemies.begin(), enemies.end(), enemy), enemies.end());
		}

		void EnemyCluster::clear(){
			xDir = true;
			yDistance = -1.0;
			enemies.clear();
			initialCount = 0;
		}

		unsigned int EnemyCluster::count(){
			return enemies.size();
		}

		void EnemyCluster::tick(double dt){
			if (!initialCount)
				initialCount = count();
			if (frozen())
				return;
			double vel = speed + (initialCount - count()) * speedInc;
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
				if (lowestPoint() > 720)	// If one alien reaches the bottom of the screen, keep moving down
					yDistance = 20000.0;
				else
					yDistance -= yd;
			}

			for (auto& enemy : enemies){
				enemy->setY( enemy->getY() + yd);
				enemy->setX(enemy->getX() + xd);
				enemy->updatePosition();
			}
		}

		void EnemyCluster::freeze(){
			frozen.reset();
		}

		// Powerup : Entity

		Powerup::Powerup(double xpos, double ypos) : 
			Entity(powerup, xpos, ypos, 10.0, 1), yvel(100.0)
		{}

		PowerupType Powerup::getPowerupType(){
			auto& rng = RNG::RNG::getInstance();
			if (rng->chanceOutOf(1, 5))	
				return slowdown;
			if (rng->chanceOutOf(1, 4))	
				return speedUp;
			if (rng->chanceOutOf(1, 3))	
				return bulletSpeedUp;
			if (rng->chanceOutOf(1, 2))	
				return fireRateUp;
			return damageUp;
		}

		void Powerup::tick(double dt){
			ypos += yvel * dt;
			yvel += 50.0 * dt;
			updatePosition();
		}
	}
}
