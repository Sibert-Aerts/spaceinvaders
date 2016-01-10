#include "StdAfx.h"
#include "level.h"

namespace SI {
	namespace Md {

	// LevelEntity

		LevelEntity::LevelEntity(EntityType type, unsigned int x, unsigned int y, unsigned int health) :
			type(type), x(x), y(y), health(health) {}

		std::shared_ptr<Entity> LevelEntity::makeEntity() const {
			switch (type) {
			case smallEnemy:
				return std::make_shared<SmallEnemy>(x * 40 + 40, y * 40 + 20, health);
				break;
			case bigEnemy:
				return std::make_shared<BigEnemy>(x * 40 + 40, y * 40 + 20, health*2);
				break;
			case barrier:
				return std::make_shared<Barrier>(x * 40 + 20, y * 40 + 20, health);
				break;
			default:
				throw(std::runtime_error("Attempted to generate invalid Entity from LevelEntity: " + std::to_string(type)));
			}
		}

	// Level
		
		Level::Level() {}

		double Level::getSpeed() const{
			return speed;
		}

		double Level::getSpeedInc() const{
			return speedInc;
		}

		std::string Level::getName() const{
			return name;
		}

		void Level::parseLevel(std::ifstream& file) {

			std::string line;
			unsigned int x(0), y(0);

			// Parse the entities
			for (;y < 15; ++y) {
				if(file.eof())
					throw(bad_parse_exception("Invalid number of lines: " + std::to_string(y)));

				std::getline(file, line);
				if (line.length() != 20)
					throw(bad_parse_exception("Invalid line length: line " + std::to_string(y)));

				for (unsigned int x = 0; x < 20; ++x) {
					if (line[x] == '.') {							// Parse nothing
						continue;
					}
					else if (line[x] == 'e' || line[x] == 'E') {	// Parse an enemy
						unsigned int health = line[x + 1] - '0';
						if (health < 1 || health > 9)
							throw(bad_parse_exception("Invalid health value for enemy at position " + std::to_string(x) + " " + std::to_string(y)));
						levelEntities.push_back(LevelEntity(line[x] == 'e'? smallEnemy : bigEnemy, x, y, health));
						++x;
					}
					else if (line[x] >= '1' && line[x] <= '9') {	// Parse a barrier
						levelEntities.push_back(LevelEntity(barrier, x, y, line[x] - '0'));
					}
					else {											// Parse an exception
						throw(bad_parse_exception("Unrecognised character at position "  + std::to_string(x) + " " + std::to_string(y)));
					}
				}
			}
			// Parse the level data:
			//		Parse speed:
			if (file.eof())
				throw(bad_parse_exception("Speed attribute missing."));

			std::getline(file, line);
			std::string speedString = parseAttribute(line, "Speed");

			std::string::size_type sz; // Parse the string as two doubles
			try {
				speed = std::stod(speedString, &sz);
				speedInc = std::stod(speedString.substr(sz));
			}
			catch (std::exception& e) {	// If it couldn't be parsed as 2 doubles throw the entire level
				std::cout << e.what();
				throw(bad_parse_exception("Couldn't parse speed attribute"));
			}

			//		Parse name:
			if (file.eof())
				throw(bad_parse_exception("Name attribute missing."));
			std::getline(file, line);
			name = parseAttribute(line, "Name");

			file.close();
		}

		std::string Level::parseAttribute(std::string line, std::string attribute){
			std::vector<std::string> bits = split(line, ':');

			if (bits[0] != attribute)
				throw(bad_parse_exception("Unrecognised attribute \"" + bits[0] + "\"."));
			if (bits.size() != 2)
				throw(bad_parse_exception("Couldn't parse " + attribute));
			return bits[1];
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
			
			std::string prefix = "Assets/levels/level";
			std::string suffix = ".txt";

			std::vector<std::shared_ptr<Level>> out;

			for (int i = 0; i < 99; ++i) {
				std::string filename = prefix + std::to_string(i) + suffix;
				std::ifstream file;
				file.open(filename.c_str());
				if (!file) {
					std::cout << "Finished reading " << i << " level files...\n";
					break;
				}
				
				try {
					const auto& level = std::make_shared<Level>();
					level->parseLevel(file);
					out.push_back(level);
				}
				catch (bad_parse_exception& e) {
					std::cout << std::endl << "Parse error encountered in level file \"" << filename << "\"." << std::endl;
					std::cout << "Error: " << e.what() << std::endl;
					std::cout << "Level not added to queue." << std::endl << std::endl;
				}
			}
			std::cout << out.size() << " levels successfuly enqued!" << std::endl;
			return out;
		}

		bad_parse_exception::bad_parse_exception(const char * message) : std::runtime_error(message) {}
		bad_parse_exception::bad_parse_exception(std::string message) : std::runtime_error(message.c_str()) {}
	}
}
