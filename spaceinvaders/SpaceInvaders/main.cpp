// SpaceInvaders.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "game.h"

using namespace SI;

int main()
{
	try {
		std::cout << "Space Invaders" << std::endl;
		
		Game game;

		std::shared_ptr<Vw::View> view = std::make_shared<Vw::View>(800, 720, "Scherm", 0.001f);

		game.registerView(view);

		game.run();

	} catch (std::exception& e) {
		std::cout << e.what();
		return 1;
	}

	return 0;


}

