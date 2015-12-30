#pragma once
#include "StdAfx.h"
#include "tools.h"
#include "entity.h"

namespace SI {
	namespace Md {

		class Entity;

		class bad_parse_exception : public std::exception {
		public:
			bad_parse_exception(const char* message);
			bad_parse_exception(std::string message);
		};

		// A struct representing an object loaded in from a level file
		struct LevelEntity {
			EntityType type;
			unsigned int health;
			unsigned int x, y;

			LevelEntity(EntityType type, unsigned int x, unsigned int y, unsigned int health );

			// Make a new Entity based off this entity's variables
			// Note: Returns a new entity each time
			std::shared_ptr<Entity> makeEntity();

		};

		// A class representing a parsed level, ready to create a vector of entities
		class Level {
		private:
			std::vector<LevelEntity> levelEntities;

			// Parse an entity based off a bit of text and add it to the vector
			void parseEntity(const char * text, unsigned int x, unsigned int y);

		public:
			double speed, speedInc;
			std::string name;

			Level();

			// Parse an entire file into a set of entities
			void parseLevel(std::ifstream& file);

			std::string parseAttribute(std::string line, std::string attribute);

			// Make a new entity vector based off the parsed levelEntities
			// Note: Returns a unique vector of entities each time, only use once per level load
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