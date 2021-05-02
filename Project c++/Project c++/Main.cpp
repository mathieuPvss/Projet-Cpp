#include <iostream>
#include "Game.h"



int main()
{
	//Init srand
	std::srand(static_cast<unsigned>(time(NULL)));

	//init de la classe game
	Game game;

	
	while (game.running() && !game.getEndGame())
	{
		
		game.update();
		game.render();
	}

	return 0;
}