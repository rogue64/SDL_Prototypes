#include "Game.h"

int main(int argc, char** argv) {
	Game* Pong = new Game;

	if (!Pong->initialize())
		return -1;

	Pong->runLoop();

	Pong->shutdown();

	return 0;
}
