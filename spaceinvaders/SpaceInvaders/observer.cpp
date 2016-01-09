#include "StdAfx.h"
#include "observer.h"

namespace SI {
	namespace Md {

		// Event

		Event::Event(EventType type, double x, double y, std::string text) :
			type(type), x(x), y(y), text(text) {}

		double Event::getX(){
			return x;
		}

		double Event::getY(){
			return y;
		}

		EventType Event::getType(){
			return type;
		}

		std::string Event::getText(){
			return text;
		}
		
		// Payload

			// Getters & Setters

		std::string ModelObserver::getLevelName()const {
			return levelName;
		}

		void ModelObserver::updateLevelName(std::string levelName){
			this->levelName = levelName;
		}

		unsigned int ModelObserver::getSecondsPassed() const {
			return secondsPassed;
		}

		void ModelObserver::updateSecondsPassed(unsigned int secondsPassed) {
			this->secondsPassed = secondsPassed;
		}

		int ModelObserver::getLives() const {
			return lives;
		}

		void ModelObserver::updateLives(int lives) {
			this->lives = lives;
		}

		ModelState ModelObserver::getState() const {
			return state;
		}

		void ModelObserver::updateState(ModelState state) {
			this->state = state;
		}

		bool ModelObserver::isPlayerInvinc()const {
			return playerInvinc;
		}

		void ModelObserver::updatePlayerInvinc(bool playerInvinc) {
			this->playerInvinc = playerInvinc;
		}

		bool ModelObserver::isPlayerDead()const {
			return playerDead;
		}

		void ModelObserver::updatePlayerDead(bool playerDead) {
			this->playerDead = playerDead;
		}

		unsigned int ModelObserver::getEntityCount(){
			return entityCount;
		}

		void ModelObserver::updateEntityCount(unsigned int entityCount) {
			this->entityCount = entityCount;
		}

		const std::vector<std::shared_ptr<EntityObserver>>& ModelObserver::getEntityObservers() const{
			return entityObservers;
		}
		
			// Events & Entities

		void ModelObserver::addEvent(Event event) {
			events.push_back(event);
		}

		std::vector<Event> ModelObserver::popEvents() {
			std::vector<Event> out = events;
			events.clear();
			return out;
		}

		std::shared_ptr<EntityObserver> ModelObserver::addEntity() {
			auto newEntity = std::make_shared<EntityObserver>();
			entityObservers.push_back(newEntity);
			return newEntity;
		}

		void ModelObserver::deleteEntity(std::shared_ptr<EntityObserver> e) {
			entityObservers.erase(std::remove(entityObservers.begin(), entityObservers.end(), e), entityObservers.end());
		}

		void ModelObserver::clearEntities(){
			entityObservers.clear();
		}

		ModelObserver::ModelObserver() :
			secondsPassed(0), 
			state(ModelState::running),
			playerDead(false)
		{}
		
		// PayloadEntity

		EntityObserver::EntityObserver() {}

		EntityType EntityObserver::getType() const {
			return type;
		}

		void EntityObserver::updateType(EntityType type){
			this->type = type;
		}

		double EntityObserver::getXpos()const {
			return xpos;
		}

		void EntityObserver::updatePosition(double xpos, double ypos) {
			this->xpos = xpos;
			this->ypos = ypos;
		}

		double EntityObserver::getYpos() const {
			return ypos;
		}

		int EntityObserver::getHealth() const {
			return health;
		}

		void EntityObserver::updateHealth(int health) {
			this->health = health;
		}

	}
}