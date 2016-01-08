#include "StdAfx.h"
#include "payload.h"

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

		std::string ModelObserver::getLevelName(){
			return levelName;
		}

		void ModelObserver::updateLevelName(std::string levelName){
			this->levelName = levelName;
		}

		unsigned int ModelObserver::getSecondsPassed() {
			return secondsPassed;
		}

		void ModelObserver::updateSecondsPassed(unsigned int secondsPassed) {
			this->secondsPassed = secondsPassed;
		}

		int ModelObserver::getLives() {
			return lives;
		}

		void ModelObserver::updateLives(int lives) {
			this->lives = lives;
		}

		ModelState ModelObserver::getState() {
			return state;
		}

		void ModelObserver::updateState(ModelState state) {
			this->state = state;
		}

		bool ModelObserver::isPlayerInvinc(){
			return playerInvinc;
		}

		void ModelObserver::updatePlayerInvinc(bool playerInvinc) {
			this->playerInvinc = playerInvinc;
		}

		bool ModelObserver::isPlayerDead(){
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

		std::vector<std::shared_ptr<EntityObserver>>& ModelObserver::getEntityObservers(){
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

		EntityType EntityObserver::getType() {
			return type;
		}

		void EntityObserver::setType(EntityType type){
			this->type = type;
		}

		double EntityObserver::getXpos() {
			return xpos;
		}

		void EntityObserver::setPos(double xpos, double ypos) {
			this->xpos = xpos;
			this->ypos = ypos;
		}

		double EntityObserver::getYpos() {
			return ypos;
		}

		int EntityObserver::getHealth() {
			return health;
		}

		void EntityObserver::setHealth(int health) {
			this->health = health;
		}

	}
}