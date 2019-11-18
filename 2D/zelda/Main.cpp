#include "Game.h"

int main(int argc, char** argv) {
	Game* Zelda = new Game;

	if (!Zelda->initialize())
		return -1;

	Zelda->runLoop();

	Zelda->shutdown();

	return 0;
}
