// SpaceInvaders.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "game.h"

using namespace SI;

int main()
{
	try {

		std::cout << "Space Invaders" << std::endl;

		std::cout << "creating game..." << std::endl;
		Game game;

		std::cout << "creating view..." << std::endl;
		std::shared_ptr<Vw::View> view = std::make_shared<Vw::View>(0.0f);

		std::cout << "registering view..." << std::endl;
		game.registerView(view);

		std::cout << "running...!" << std::endl;
		game.run();
		std::cout << "Done!" << std::endl;

	} catch (std::exception& e) {
		std::cout << "\nException encountered!" << std::endl;
		std::cout << e.what() << std::endl;
		return 1;
	}

	return 0;


}

