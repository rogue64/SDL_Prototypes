#pragma once
#include "SDL\SDL.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Random.h"
#include "Block.h"
#include "Paddle.h"
#include "Ball.h"
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <SDL\SDL_image.h>
#include <vector>
#include <unordered_map>

// forward declarations of classes
class Actor;
class Block;
class Paddle;
class Ball;

//constants
static const int WINDOW_W = 1024;
static const int WINDOW_H = 768;
static const int SEC = 1000; // a second in ms
static const int FPS = 60;
static const int DELTA_MIN = SEC / FPS;
static const float DELTA_CAP = 0.033;
static const int CELL_W = 64;
static const int CELL_H = 32;
static const int WALL_OSET = 32;
static const float PADDL_SPD = 700;


class Game {
private:
	// window for game
	SDL_Window* my_window = nullptr;

	// renderer for window of game
	SDL_Renderer* my_renderer = nullptr;

	// variables for frame limiting
	size_t prev_time = 0, curr_time = 0;
	float delt_time = 0;
	
	// bool to keep track if game is off or not
	bool off = false;

	// vector of Actor pointers
	std::vector<Actor*> my_actors;
	std::vector<Actor*> new_actors;

	// game's paddle
	Paddle* my_paddle = nullptr;

	// game's ball
	Ball* my_ball = nullptr;

	// vector of SpriteComponent pointers
	std::vector<SpriteComponent*> my_sprites;

	// hash map of textures loaded for the game
	std::unordered_map<std::string, SDL_Texture*> textureMap;

	// reads input, called in game loop
	void processInput();
	
	// updates game-state, called in game loop
	void updateGame();
	
	// shows effects of current game-state, called in game loop
	void generateOutput();

	// load and unload actors to my_vector
	void LoadData();
	void UnloadData();


public: 
	// constructor
	Game();

	// initializer
	bool initialize();

	// shuts down game
	void shutdown();

	// runs game loop
	void runLoop();

	// returns pointer to the game's paddle
	Paddle* getPaddle() const { return my_paddle; }

	// adds given actor game
	void addActor(Actor* actor);

	// removes Actor matching given pointer
	void removeActor(Actor* actor);

	// adds given spriteComponent to game
	void addSprite(SpriteComponent* sprite);

	// removes sprite given matching pointer
	void removeSprite(SpriteComponent* sprite);

	// returns texture from given file
	SDL_Texture* getTexture(std::string filename);
};