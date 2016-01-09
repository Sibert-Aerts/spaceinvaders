#pragma once
#include "StdAfx.h"
#include "tools.h"
#include "entity.h"

namespace SI {
	namespace Md {

		class Entity;

		// An exception thrown when the read input does not follow the input format
		class bad_parse_exception : public std::exception {
		public:
			bad_parse_exception(const char* message);
			bad_parse_exception(std::string message);
		};

		// A struct representing an object loaded in from a level file
		struct LevelEntity {
			// The type of entity it is, limited to smallEnemy, bigEnemy and barrier
			EntityType type;

			// The amount of health it should spawn with
			unsigned int health;

			// Its position on the 20 x 18 grid
			unsigned int x, y;

			LevelEntity(EntityType type, unsigned int x, unsigned int y, unsigned int health );

			// Create a new entity based off the data stored in the LevelEntity
			std::shared_ptr<Entity> makeEntity() const;

		};

		// A class representing a parsed level, ready to create a vector of entities
		class Level {
		private:
			// The speed and speedInc for the EnemyCluster, as read from the level file
			double speed, speedInc;
			// The level's name as read from the level file
			std::string name;

			// A vector of LevelEntities
			std::vector<LevelEntity> levelEntities;

		public:
			Level();

			// Get the EnemyCluster's speed
			double getSpeed()const;

			// Get the EnemyCluster's speedInc
			double getSpeedInc()const;

			// Get the Level's name
			std::string getName() const; 
			
			// Parse a level file
			void parseLevel(std::ifstream& file);

			// Parse an attribute from a level file
			std::string parseAttribute(std::string line, std::string attribute);

			// Create a new vector of new Entities based off the parsed level file
			std::vector<std::shared_ptr<Entity>> makeEntities();


		};

		// A class designed to parse level files and generate a vector of levels
		class LevelParser {
		public:

			LevelParser();

			// Parse all level files and return it as a vector of levels
			std::vector<std::shared_ptr<Level>> parseLevels();

		};
		

	}
}