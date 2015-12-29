#include "StdAfx.h"
#include "level.h"

namespace SI {
	namespace Md {

	// LevelEntity

		LevelEntity::LevelEntity(EntityType type, unsigned int x, unsigned int y, unsigned int health) :
			type(type), x(x), y(y), health(health) {}

		std::shared_ptr<Entity> LevelEntity::makeEntity(){
			switch (type) {
			case enemy:
				return std::make_shared<Enemy>(x * 40 + 40, y * 40 + 20, health);
				break;
			case barrier:
				return std::make_shared<Barrier>(x * 40 + 20, y * 40 + 20, health);
				break;
			default:
				throw(std::runtime_error("Attempted to generate invalid Entity from LevelEntity: " + std::to_string(type)));
			}
		}

	// Level
		
		void Level::parseEntity(const char * text, unsigned int x, unsigned int y) {
			return;
		}

		Level::Level() {}

		void Level::parseLevel(const char * fileName) {

			std::ifstream file;
			file.open(fileName);

			std::string line;
			unsigned int x(0), y(0);

			while (!file.eof() && y < 15) {
				std::getline(file, line);
				if (line.length() != 20)
					throw(bad_parse_exception("Attempted to parse line of improper length: line " + std::to_string(y)));

				for (unsigned int x = 0; x < 20; ++x) {
					if (line[x] == '.') {
						continue;
					}
					else if (line[x] == 'e') {
						unsigned int health = line[x + 1] - '0';
						if (health < 1 || health > 9)
							throw(bad_parse_exception("Invalid health value for enemy at position " + std::to_string(x) + " " + std::to_string(y)));
						levelEntities.push_back(LevelEntity(enemy, x, y, health));
						++x;
					}
					else if (line[x] >= '1' && line[x] <= '9') {
						levelEntities.push_back(LevelEntity(barrier, x, y, line[x] - '0'));
					}
					else {
						throw(bad_parse_exception(std::string("Unrecognised character \"") + line[x] + std::string("\".")));
					}
				}
				++y;
			}
			file.close();
		}

		std::vector<std::shared_ptr<Entity>> Level::makeEntities() {

			std::vector<std::shared_ptr<Entity>> out;
			for (auto& le : levelEntities)
				out.push_back(le.makeEntity());

			return out;
		}
		
	// LevelParser

		LevelParser::LevelParser(){}

		std::vector<std::shared_ptr<Level>> LevelParser::parseLevels(){

			// Todo: replace this with a smarter system
			std::vector<std::string> files = { "level0.txt" ,"level1.txt" ,"level2.txt","level3.txt" };
			std::string folder = "Assets\\levels\\";

			std::vector<std::shared_ptr<Level>> out;

			for (std::string& file : files) {
				try {
					auto& level = std::make_shared<Level>();
					level->parseLevel((folder + file).c_str());
					out.push_back(level);
				}
				catch (bad_parse_exception& e) {
					std::cout << std::endl << "Parse error encountered in level file \"" << folder << file << "\"." << std::endl;
					std::cout << "Error: " << e.what() << std::endl;
					std::cout << "Level not added to queue." << std::endl << std::endl;
				}
			}

			return out;
		}

		bad_parse_exception::bad_parse_exception(const char * message) : std::exception(message) {}
		bad_parse_exception::bad_parse_exception(std::string message) : std::exception(message.c_str()) {}
	}
}