#pragma once

#include "StdAfx.h"
#include "Time.h"

namespace SI
{
	namespace Md {

	// Helper functions:

		// A function that calculates the resulting velocity of an object after collission
		inline double elasticCollission(double v1, double m1, double v2, double m2) {
			return (v1*(m1 - m2) + 2 * m2*v2) / (m1 + m2);
		}



	// Classes:

	// An abstract class for an Entity in the world
		class Entity {
		public:
			double xpos, ypos;
			float size;

			Entity(double xpos = 0, double ypos = 0, float size = 10.0f) : xpos(xpos), ypos(ypos), size(size) {}

			// TODO: replace with actual virtual function so we can dynamic cast
			virtual void toet() {
				std::cout << "BASE TOET\n";
			}

		};

	// An entity that bounces around and collides with other DebugEntity
	// Implemented to test the view-model system
		class DebugEntity : public Entity {
		public:
			double xvel, yvel;
			double xacc, yacc;
			float size;

			DebugEntity(double xpos = 0, double ypos = 0, double xvel = 0, double yvel = 0, double xacc = 0, double yacc = 0, float size = 20.0f) : 
				Entity(xpos,ypos), 
				xvel(xvel), yvel(yvel),
				xacc(xacc), yacc(yacc),
				size(size)
				{}

			void tick(double dt) {
				xvel += (dt * xacc);
				yvel += (dt * yacc);
				xpos += (dt * xvel);
				ypos += (dt * yvel);
			}

			bool collide(std::shared_ptr<DebugEntity>& e) {
				double D = sqrt(pow((xpos - e->xpos), 2) + pow((ypos - e->ypos), 2));
				double d =  D - (size + e->size);
				if ( d > 0 )
					return false;
				
				double tempx (xvel), tempy(yvel);
				if(xpos < e-> xpos)
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

		};
	
	// An entity representing the player
		class Player : public Entity {
		public:
			std::string name;
			Time::BinaryRepeatTimer fireCooldown;

			Player(double x, double y, std::string name) : 
				Entity(x,y),
				name(name),
				fireCooldown(0.4f)
				// TODO: shove the model's clockwatch in here, or shove this up the model
				// (ideally the prior)
				{}

			void speak() {
				std::cout << name << std::endl;
			}

		};


		class Enemy : public Entity {
		public:
			Enemy(double x, double y, float size = 20.0f) :
				Entity(x, y, size)
			{}


		};

		class Bullet : public Entity {
		public:
			double xvel, yvel;
			float size;

			Bullet(double xpos, double ypos, double xvel, double yvel, float size = 5.0f) :
				Entity(xpos, ypos),
				xvel(xvel), yvel(yvel),
				size(size)
			{}

			void tick(double dt) {
				xpos += xvel * dt;
				ypos += yvel * dt;
			}

			bool collide(std::shared_ptr<Enemy>& enemy) {
				double D = sqrt(pow((xpos - enemy->xpos), 2) + pow((ypos - enemy->ypos), 2));
				double d = D - (size + enemy->size);
				if (d > 0)
					return false;
				return true;
			}
		};
	}
}