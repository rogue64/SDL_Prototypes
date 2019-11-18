#pragma once
#include "constants.h"
#include "SDL\SDL.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "Random.h"
#include "Block.h"
#include "Player.h"
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
class Player;

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

	// game's camera position
	Vector2 my_camera = Vector2::Zero;

	// vector of Actor pointers
	std::vector<Actor*> my_actors;
	std::vector<Actor*> new_actors;

	// vector of pointers to loaded blocks (for collsions)
	std::vector<Block*> my_blocks;

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

	// adds given actor game
	void addActor(Actor* actor);

	// removes Actor matching given pointer
	void removeActor(Actor* actor);

	// returns vector of game's block's pointers
	const std::vector<Block*>& getBlocks() const { return my_blocks; }

	// adds given block to game's vector
	void addBlock(Block* block);

	// deletes given block from vector
	void removeBlock(Block* block);

	// adds given spriteComponent to game
	void addSprite(SpriteComponent* sprite);

	// removes sprite given matching pointer
	void removeSprite(SpriteComponent* sprite);

	// returns texture from given file
	SDL_Texture* getTexture(std::string filename);

	// accesses the game's camera variable
	Vector2& getCamera() { return my_camera; }
};